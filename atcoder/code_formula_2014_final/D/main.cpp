#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), lim##i = (b); i < lim##i; i++)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

i64 solve() {
  int n;
  cin >> n;
  vector<int> H(n);
  cin >> H;
  vector<tuple<int, int, int>> movies(n);
  vector<int> previous(n, -1), previous2(n, -1);

  REP(i, n) {
    int m, s, e;
    cin >> m >> s >> e;
    movies[i] = {e, s, m};
  }
  sort(ALL(movies));

  {
    const int M = 3005;
    vector<map<int, int>> movie_end(M);
    REP(i, n) {
      auto [e, s, m] = movies[i];

      auto it = movie_end[m].upper_bound(s);
      if (it != movie_end[m].begin()) {
        --it;
        previous[i] = it->second;
      }

      auto jt = movie_end[m].upper_bound(e);
      if (jt == movie_end[m].begin()) {
        movie_end[m][e] = i;
      } else {
        --jt;
        int pstart = get<1>(movies[jt->second]);
        if (pstart < s) {
          movie_end[m][e] = i;
        }
      }
    }
  }

  {
    map<int, int> movie_end;
    REP(i, n) {
      auto [e, s, m] = movies[i];
      auto it = movie_end.upper_bound(s);
      if (it != movie_end.begin()) {
        --it;
        previous2[i] = it->second;
      }
      movie_end[e] = i;
    }
  }

  vector dp(n + 1, vector(n + 1, 0));
  vector best(n + 1, 0);
  REP(i, n) {
    best[i + 1] = dp[i + 1][0] = best[i];

    int base_score = 0;
    int q = previous2[i];
    if (q >= 0) {
      base_score = best[q + 1];
    }
    dp[i + 1][1] = base_score + H[0];
    int best_j = 0;
    if (chmax(best[i + 1], dp[i + 1][1])) {
      best_j = 1;
    }

    int depth = 0;
    for (int x = previous[i]; x >= 0; x = previous[x]) {
      ++depth;
    }
    if (depth > 0) {
      int p = previous[i];
      for (int j = depth + 1; j >= 2; --j) {
        chmax(dp[i + 1][j], dp[p + 1][j - 1] + H[j - 1]);
        if (chmax(best[i + 1], dp[i + 1][j])) {
          best_j = j;
        }
      }
    }
    DEBUG(i + 1, best[i + 1], best_j);
  }
  return best[n];
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
