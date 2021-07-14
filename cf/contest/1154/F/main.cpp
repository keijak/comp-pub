#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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

i64 solve() {
  int n, m, k;
  cin >> n >> m >> k;
  V<int> a(n);
  cin >> a;
  V<int> offers(k + 1);
  REP(i, m) {
    int x, y;
    cin >> x >> y;
    if (x > k) continue;
    chmax(offers[x], y);
  }
  sort(ALL(a));
  V<i64> rcum(n + 1);
  REP(i, n) { rcum[n - 1 - i] = rcum[n - i] + a[n - 1 - i]; }

  DEBUG(a);
  DEBUG(rcum);
  const i64 INF = 1e13;
  auto dp = vector(k + 1, INF);
  dp[k] = 0;
  for (int i = k - 1; i >= 0; --i) {
    for (int j = 1; j <= i + 1; ++j) {
      int y = offers[j];
      i64 cost = rcum[i - (j - 1) + y] - rcum[i + 1];
      chmin(dp[i - j + 1], dp[i + 1] + cost);
    }
    DEBUG(i, a[i]);
    DEBUG(dp);
  }
  return dp[0];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve() << endl;
}
