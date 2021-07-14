#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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

i64 apsum(i64 x) { return x * (x + 1) / 2LL; }

i64 solve(int test_case) {
  int n;
  cin >> n;
  map<int, int> mt;
  REP(i, n) {
    int t;
    cin >> t;
    mt[t]++;
  }
  const int K = mt.size();
  V<pair<int, int>> times(K);
  {
    auto it = mt.begin();
    REP(i, K) {
      times[i] = *it;
      ++it;
    }
  }

  auto calc_cost = [&](int k, int l) -> i64 {
    auto [ct, cnt] = times[k];
    int r = l + cnt - 1;
    i64 cost = 0;
    if (l <= ct) {
      cost += apsum(ct - l);
    } else {
      cost -= apsum(l - 1 - ct);
    }
    if (r >= ct) {
      cost += apsum(r - ct);
    } else {
      cost -= apsum(ct - (r + 1));
    }
    return cost;
  };

  const int L = 400;
  const int M = 200;
  const i64 INF = 1e18;
  auto dp = vector(L + 1, vector(K + 1, vector(M + 1, INF)));
  dp[0][0][0] = 0LL;
  for (i64 t = 1; t <= L; ++t) {
    for (int k = 0; k <= K; ++k) {
      chmin(dp[t][k][0], dp[t - 1][k][0]);
      for (int m = 0; m < M; ++m) {
        chmin(dp[t][k][m], dp[t - 1][k][m + 1]);
      }
    }
    for (int k = 1; k <= K; ++k) {
      auto [ct, cnt] = times[k - 1];
      i64 cost = calc_cost(k - 1, t);
      if (dp[t - 1][k - 1][0] < INF) {
        chmin(dp[t][k][cnt], dp[t - 1][k - 1][0] + cost);
      }
      if (dp[t - 1][k - 1][1] < INF) {
        chmin(dp[t][k][cnt], dp[t - 1][k - 1][1] + cost);
      }
    }
  }
  return dp[L][K][0];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int q;
  cin >> q;
  REP(i, q) { cout << solve(i) << '\n'; }
}
