#include <bits/stdc++.h>

#include <atcoder/modint>

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
using Mint = atcoder::modint1000000007;

// nCk where n can be large but min(k, n-k) is small.
Mint comb(i64 n, i64 k) {
  i64 p = std::min(k, n - k), q = std::max(k, n - k);
  assert(p >= 0);
  Mint nume = 1, deno = 1;
  for (i64 i = n; i > q; --i) nume *= i;
  for (i64 i = 2; i <= p; ++i) deno *= i;
  return nume / deno;
}

i64 solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  cin >> a;
  Mint ans = 1;
  int l = 0, r = 0;
  while (true) {
    assert(l < n and a[l] != -1);
    r = l + 1;
    while (r < n and a[r] == -1) ++r;
    if (r == n) break;
    int m = r - l;
    int k = a[r] - a[l];
    if (k < 0) return 0;
    Mint res = comb(m - 1 + k, k);
    ans *= res;
    l = r;
  }

  return ans.val();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
