#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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

#ifdef ENABLE_DEBUG
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

optional<vector<int>> solve() {
  int n, m;
  cin >> n >> m;
  vector s(n, vector(m, 0));
  REP(i, n) REP(j, m) { cin >> s[i][j]; }

  vector<int> ans(m);
  if (m <= 2) {
    REP(i, m) ans[i] = 1;  // anything is ok
    return ans;
  }

  vector<vector<pair<int, int>>> cand(m);
  REP(j, m) {
    map<int, int> counter;
    REP(i, n) { counter[s[i][j]]++; }
    for (auto [x, cnt] : counter) {
      cand[j].emplace_back(cnt, x);
    }
    sort(ALL(cand[j]), greater<>());
    DEBUG(j, cand[j]);
  }

  const int LIM = 10000000;
  int steps = 0;
  vector<int> idx(m, 0);
  auto search = [&](auto rec, int d) -> bool {
    REP(j, m) { auto [cnt, val] = cand[j][idx[j]]; }

    REP(j, m) {
      if (idx[j] >= min(d, ssize(cand[j]))) continue;
      idx[j]++;
      if (rec(rec, d + 1)) return true;
      idx[j]--;
    }
    return false;
  };

  int maxd = 2;
  while (steps < LIM) {
    if (search(search, maxd)) return ans;
    ++maxd;
  }

  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (not res.has_value()) {
    cout << "No\n";
  } else {
    cout << "Yes\n";
    print_seq(res.value());
  }
}
