#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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
  vector<pair<i64, i64>> commands(n);
  REP(i, n) {
    i64 t, x;
    cin >> t >> x;
    commands[i] = {t, x};
  }
  i64 res = 0;
  i64 nt = -1;
  i64 ntpos = 0;
  i64 pos = 0;
  i64 lt = 0;
  REP(i, n) {
    auto [t, x] = commands[i];
    if (t < nt) {
      // ignored
      i64 d;
      if (ntpos > pos) {
        d = 1;
      } else if (ntpos < pos) {
        d = -1;
      } else {
        d = 0;
      }
      i64 cur = pos + d * (t - lt);
      i64 fut;
      if (i == n - 1 or commands[i + 1].first >= nt) {
        fut = ntpos;
      } else {
        fut = pos + d * (commands[i + 1].first - lt);
      }
      if (d > 0 and cur <= x and x <= fut) {
        ++res;
        DEBUG(i, res, "+");
      } else if (d < 0 and fut <= x and x <= cur) {
        ++res;
        DEBUG(i, res, "-");
      }
    } else {
      pos = ntpos;
      i64 d = x - pos;
      if (d >= 0) {
        nt = t + d;
      } else {
        nt = t - d;
      }

      if (i == n - 1 or commands[i + 1].first >= nt) {
        DEBUG(i, res);
        ++res;
      }

      ntpos = x;
      lt = t;
    }
  }
  return res;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << solve() << "\n"; }
}
