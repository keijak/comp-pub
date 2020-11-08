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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  V<i64> a(n);
  cin >> a;
  V<i64> lcum(n + 1), rcum(n + 1);  // [0, i), [i, n)
  REP(i, n) { lcum[i + 1] = lcum[i] + a[i]; }
  REP(i, n) {
    int ri = n - 1 - i;
    rcum[ri] = rcum[ri + 1] + a[ri];
  }
  i64 total = accumulate(ALL(a), 0LL);

  const i64 INF = 1e18;
  i64 ans = INF;

  REP(i, n) {
    i64 p = rcum[i];
    if (p <= total / 2) {
      auto it = upper_bound(lcum.begin(), lcum.begin() + i, total / 2 - p);
      if (it != lcum.end()) {
        i64 x = p + *it;
        i64 y = total - x;
        if (y >= 0) {
          i64 d = abs(x - y);
          chmin(ans, d);
        }
      }
      if (it != lcum.begin()) {
        --it;
        i64 x = p + *it;
        i64 y = total - x;
        if (y >= 0) {
          i64 d = abs(x - y);
          chmin(ans, d);
        }
      }
    } else {
      auto it = upper_bound(lcum.begin() + i, lcum.end(), total / 2 + lcum[i]);
      if (it != lcum.end()) {
        i64 x = *it - lcum[i];
        i64 y = total - x;
        if (y >= 0) {
          i64 d = abs(x - y);
          chmin(ans, d);
        }
      }
      if (it != lcum.begin()) {
        --it;
        i64 x = *it - lcum[i];
        i64 y = total - x;
        if (y >= 0) {
          i64 d = abs(x - y);
          chmin(ans, d);
        }
      }
    }
  }
  cout << ans << endl;
}
