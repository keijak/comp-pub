#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

// Returns a vector that maps x to x's smallest prime factor (> 1).
// O(n log log n)
std::vector<int> sieve_smallest_prime_factors(int n) {
  std::vector<int> res(n + 1);
  for (int i = 1; i <= n; ++i) res[i] = i;
  for (int i = 4; i <= n; i += 2) res[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (res[i] != i) continue;
    for (int j = i * i; j <= n; j += i) {
      if (res[j] == j) res[j] = i;
    }
  }
  return res;
}

// Quick factorization.
std::vector<std::pair<int, int>> quick_factorize(int n,
                                                 const std::vector<int> &spf) {
  assert(0 < n and n < int(spf.size()));
  std::vector<std::pair<int, int>> res;
  for (;;) {
    int p = spf[n];
    if (p == 1) break;
    int count = 0;
    do {
      n /= p;
      ++count;
    } while (n % p == 0);
    res.emplace_back(p, count);
  }
  return res;
}

// Returns all prime numbers smaller than or equal to n.
std::vector<int> primes_upto(int n) {
  std::vector<int> res;
  const auto &spf = sieve_smallest_prime_factors(n);
  for (int i = 2; i <= n; ++i) {
    if (spf[i] == i) res.push_back(i);
  }
  return res;
}

#include <boost/multiprecision/cpp_int.hpp>
// using Int = boost::multiprecision::cpp_int;
using Int = boost::multiprecision::int256_t;
using i512 = boost::multiprecision::int512_t;
using i1024 = boost::multiprecision::int1024_t;

// using Int = __int128_t;
const int L = 100000;
const int AMAX = int(1e9) + 5;
const int MOD = int(1e9) + 7;

Mint solve() {
  auto primes = primes_upto(L);
  int m;
  cin >> m;
  vector<int> a(m);
  cin >> a;
  DEBUG(m, a);
  map<int, int> fac;
  REP(i, m) {
    int x = a[i];
    for (auto p : primes) {
      if (x % p != 0) continue;
      int count = 0;
      while (x % p == 0) {
        x /= p;
        ++count;
      }
      fac[p] += count;
    }
    if (x > 1) {
      fac[x] += 1;
    }
  }
  DEBUG(fac);

  Int num_factors = 1;
  for (const auto &[p, k] : fac) {
    num_factors *= (k + 1) % (MOD - 1);
    num_factors %= MOD - 1;
  }
  Mint total = Mint(2).pow(i64(num_factors));

  const int P = ssize(fac);
  vector dp(P + 1, vector(P + 1, Mint(0)));
  {
    dp[0][0] = 1;
    int i = 0;
    for (const auto &[p, k] : fac) {
      // Don't use p.
      for (int j = 0; j <= P; ++j) {
        dp[i + 1][j] = dp[i][j];
      }
      // Add to an existing group.
      for (int j = 1; j < P; ++j) {
        dp[i + 1][j] += dp[i][j] * Mint(k) * j;
      }
      // Create a new group.
      for (int j = 0; j < P; ++j) {
        dp[i + 1][j + 1] += dp[i][j] * k;
      }
      ++i;
    }
  }
  Mint ans = total;
  REP(i, P + 1) { ans -= dp[P][i] * 2; }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
