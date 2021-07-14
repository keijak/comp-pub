#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
using f128 = long double;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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

double solve() {
  i64 n, l, v1, v2, k;
  cin >> n >> l >> v1 >> v2 >> k;

  auto check = [&](double x) {
    // cerr << "===== x = " << x << " ====== \n";
    if (v1 * x >= l) return true;
    int rem = n;
    double t2 = (l - v1 * x) / double(v2 - v1);
    double t0 = 0;
    for (int i = 0;; ++i) {
      if (t0 + t2 > x) return false;
      rem -= k;
      DEBUG(i, rem);
      if (rem <= 0) return true;
      double tret = (v2 - v1) * t2 / (v1 + v2);
      t0 += t2 + tret;
      DEBUG(tret, t0);
      if (t0 > x) return false;
    }
  };

  double fv = 0.0, tv = n * 2 * double(l) / v2;
  REP(iter, 80) {
    double mid = (fv + tv) * 0.5;
    if (check(mid)) {
      // cerr << "    OK\n";
      tv = mid;
    } else {
      // cerr << "    NG\n";
      fv = mid;
    }
  }
  return tv;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << fixed << setprecision(16);
  cout << solve() << endl;
}
