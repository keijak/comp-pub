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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

optional<pair<string, string>> solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  assert(ssize(s) == n);
  if (s[0] == '0') return nullopt;
  int count0 = 0;
  REP(i, n) {
    if (s[i] == '0') ++count0;
  }
  if (count0 % 2 == 1) return nullopt;
  string a(n, ' '), b(n, ' ');
  a[0] = b[0] = '(';
  vector<int> acc(n + 1, 0);
  for (int i = n - 1; i >= 0; --i) {
    acc[i] = acc[i + 1] + (s[i] == '1' ? 1 : 0);
  }
  int ca = 1, cb = 1;
  REP(i, 1, n) {
    if (s[i] == '1') {
      int r = acc[i] * 2;
      if (ca + cb < r) {
        a[i] = b[i] = '(';
        ++ca, ++cb;
      } else {
        a[i] = b[i] = ')';
        --ca, --cb;
      }
    } else {
      if (ca == cb) {
        a[i] = '(';
        b[i] = ')';
        ++ca, --cb;
      } else {
        assert(ca > cb);
        a[i] = ')';
        b[i] = '(';
        --ca, ++cb;
      }
    }
    if (ca < 0 or cb < 0) return nullopt;
  }
  if (ca != 0 or cb != 0) return nullopt;
  return pair{a, b};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    auto res = solve();
    if (not res.has_value()) {
      cout << "NO\n";
    } else {
      cout << "YES\n";
      const auto &[a, b] = res.value();
      cout << a << "\n";
      cout << b << "\n";
    }
  }
}
