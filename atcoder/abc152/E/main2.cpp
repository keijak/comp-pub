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

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

// osa_k's algorithm. O(n log log n).
// Returns a vector that maps x to x's smallest divisor (> 1).
std::vector<int> min_divisor_sieve(int n) {
  std::vector<int> res(n + 1);
  for (int i = 1; i <= n; ++i) res[i] = i;
  for (int i = 4; i <= n; i += 2) res[i] = 2;
  int sqrt_n = std::lround(std::ceil(std::sqrt((double)n)));  // n in 32 bits.
  for (int i = 3; i <= sqrt_n; i += 2) {
    if (res[i] != i) continue;
    for (int j = i * i; j <= n; j += i) {
      if (res[j] == j) res[j] = i;
    }
  }
  return res;
}

// Quick factorization.
map<int, int> quick_factorize(int n, const std::vector<int> &min_divisor) {
  assert(0 < n and n < int(min_divisor.size()));
  map<int, int> res;
  for (;;) {
    int p = min_divisor[n];
    if (p == 1) break;
    int count = 0;
    do {
      n /= p;
      ++count;
    } while (n % p == 0);
    res.emplace(p, count);
  }
  return res;
}

Mint solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  cin >> a;

  auto min_divisors = min_divisor_sieve(1'000'005);
  map<int, int> pm;
  vector<map<int, int>> afs(n);
  REP(i, n) {
    afs[i] = quick_factorize(a[i], min_divisors);
    for (auto [p, cnt] : afs[i]) {
      if (pm.count(p)) {
        chmax(pm[p], cnt);
      } else {
        pm[p] = cnt;
      }
    }
  }
  Mint alcm = 1;
  for (auto [p, cnt] : pm) {
    alcm *= Mint(p).pow(cnt);
  }
  Mint ans = 0;
  REP(i, n) {
    Mint b = alcm / a[i];
    ans += b;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
