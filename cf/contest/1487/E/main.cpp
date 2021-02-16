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

const i64 BIG = 1e16;

i64 solve() {
  array<int, 4> n;
  REP(i, 4) cin >> n[i];
  array<vector<i64>, 4> a;
  array<vector<i64>, 4> dp;
  REP(i, 4) {
    a[i].resize(n[i]);
    REP(j, n[i]) cin >> a[i][j];
    dp[i].resize(n[i], BIG);
  }

  REP(i, n[0]) dp[0][i] = a[0][i];
  array<int, 3> m;
  REP(i, 3) {
    vector<int> idx(n[i]), rev(n[i]);
    REP(j, n[i]) idx[j] = j;
    sort(ALL(idx), [&](int x, int y) { return dp[i][x] < dp[i][y]; });
    REP(j, n[i]) {
      int k = idx[j];  // dish k is j-th cheapest
      rev[k] = j;
    }

    cin >> m[i];
    vector<vector<int>> bad(n[i + 1]);
    REP(j, m[i]) {
      int x, y;
      cin >> x >> y;
      --x, --y;
      int r = rev[x];
      bad[y].push_back(r);
    }

    REP(j, n[i + 1]) {
      auto &badp = bad[j];
      if (ssize(badp) == n[i]) {
        dp[i + 1][j] = BIG;
        continue;
      }
      sort(ALL(badp));
      int p = 0;
      while (p < ssize(badp) and badp[p] == p) ++p;
      // select p-th cheapest food
      dp[i + 1][j] = a[i + 1][j] + dp[i][idx[p]];
    }
  }

  i64 ans = BIG;
  REP(i, n[3]) chmin(ans, dp[3][i]);
  return (ans == BIG) ? -1 : ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
