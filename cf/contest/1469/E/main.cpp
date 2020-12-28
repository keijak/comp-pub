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
const int LMAX = (1 << 20) + 5;
char bs[LMAX];
vector<char> s;
string ans;

bool solve() {
  int n, k;
  cin >> n >> k;
  s.resize(n);
  cin >> s;

  bool exhaust = k <= 20;
  int L = (1 << min(k, 20)) - 1;
  chmin(L, n - k + 1);
  REP(i, L + 1) bs[i] = true;
  int b_count = L + 1;

  unsigned upper = 0, lower = 0;
  const unsigned bit20 = 1 << 20;

  unsigned mask = exhaust ? (1 << k) : bit20;
  REP(i, n) {
    lower <<= 1;
    if (s[i] == '0') lower += 1;
    if (exhaust) {
      lower &= ~mask;
    } else {
      if (lower & bit20) {
        ++upper;
        lower &= ~bit20;
      }
    }
    if (i >= k - 1) {
      if (not exhaust and i - k >= 0 and s[i - k] == '0') upper--;
      if (upper == 0) {
        if (lower <= L and bs[lower]) {
          bs[lower] = false;
          b_count--;
        }
      }
    }
  }

  //   if (b_count == 0) {
  //     cout << "NO\n";
  //     return false;
  //   }
  int num = -1;
  REP(i, L + 1) {
    if (bs[i]) {
      num = i;
      break;
    }
  }
  if (num == -1) {
    cout << "NO\n";
    return false;
  }

  ans.assign(k, '0');
  for (int i = 0; i < k; ++i) {
    if (num == 0) break;
    if (num & 1) ans[i] = '1';
    num >>= 1;
  }
  reverse(ALL(ans));
  cout << "YES\n" << ans << "\n";
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) solve();
}
