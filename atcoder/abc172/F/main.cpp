#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

i64 solve() {
  int n;
  cin >> n;
  vector<u64> a(n);
  cin >> a;
  i64 a1 = a[0], a2 = a[1], a3 = 0;
  REP(i, 2, n) { a3 ^= a[i]; }
  if ((a1 ^ a2 ^ a3) == 0) return 0;
  if (a1 + a2 < a3) return -1;
  const i64 BIG = 1e17;

  map<array<u64, 3>, i64> memo;

  auto f = [&](auto &f, u64 p, u64 q, u64 r) -> i64 {
    if (p == 0 and q == 0) {
      return (r == 0) ? 0 : BIG;
    }
    array<u64, 3> mkey = {p, q, r};
    if (auto it = memo.find(mkey); it != memo.end()) {
      return it->second;
    }
    u64 p1 = p & 1;
    u64 q1 = q & 1;
    u64 r1 = r & 1;
    i64 ans = BIG;
    if ((p1 ^ q1) == r1) {
      auto x = f(f, p >> 1, q >> 1, r >> 1);
      if (x != BIG) chmin(ans, x * 2);
    }
    if (p >= 1) {
      if ((((p - 1) & 1) ^ ((q + 1) & 1)) == r1) {
        auto x = f(f, (p - 1) >> 1, (q + 1) >> 1, r >> 1);
        if (x != BIG) chmin(ans, x * 2 + 1);
      }
    }
    memo[mkey] = ans;
    return ans;
  };
  auto ans = f(f, a1, a2, a3);
  if (ans >= a1) return -1;
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
