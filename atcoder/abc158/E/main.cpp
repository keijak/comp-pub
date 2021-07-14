#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
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
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
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

template <unsigned int &M>
struct ModInt {
  constexpr ModInt(long long val = 0) : _v(0) {
    if (val < 0) {
      long long k = (abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned int umod() { return M; }
  inline unsigned int val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  constexpr ModInt operator-() const { return ModInt(-_v); }
  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long)(_v)*a._v) % M;
    return *this;
  }
  constexpr ModInt pow(unsigned long long t) const {
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // Inverse by Extended Euclidean algorithm.
  // M doesn't need to be prime, but x and M must be coprime.
  constexpr ModInt inv() const {
    assert(_v != 0);
    long long x = 0, y = 0;
    long long g = ext_gcd(_v, M, x, y);
    assert(g == 1LL);  // gcd(_v, M) must be 1.
    return x;
  }
  constexpr ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    return ModInt(a) /= b;
  }
  friend constexpr bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend constexpr bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  // Extended Euclidean algorithm
  // Returns gcd(a,b).
  // x and y are set to satisfy `a*x + b*y == gcd(a,b)`
  static long long ext_gcd(long long a, long long b, long long &x,
                           long long &y) {
    if (b == 0) {
      x = 1;
      y = 0;
      return a;
    }
    long long d = ext_gcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
  }

  unsigned int _v;  // raw value
};
unsigned int P = 1'000'000'007;
using Mint = ModInt<P>;

using namespace std;

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int n;
  cin >> n >> P;
  string S;
  cin >> S;
  assert(n == SIZE(S));

  // O(N * P)
  auto solve_naive = [&]() -> u64 {
    u64 ans = 0;
    V<u64> freq(P, 0);
    freq[0] = 1;
    REP(i, n) {
      int d = S[i] - '0';
      int q = (10 * P - d) % P;
      ans += freq[q];
      V<u64> freq_next(P, 0);
      REP(j, P) { freq_next[((j + d) * 10) % P] += freq[j]; }
      freq_next[0]++;
      swap(freq, freq_next);
    }
    return ans;
  };

  // O(N + P)
  auto solve = [&]() -> u64 {
    auto cum = make_vec(n + 1, Mint(0));
    u64 dbase = 1;
    REP(i, n) {
      int ri = n - 1 - i;
      Mint d = S[ri] - '0';
      cum[i + 1] = cum[i] + (d * dbase);
      dbase = (dbase * 10) % P;
    }
    u64 ans = 0;
    V<u64> freq(P, 0);
    REP(i, n + 1) {
      int ri = n - i;
      u64 cnt = freq[cum[ri].val()];
      ans += cnt;
      freq[cum[ri].val()]++;
    }
    return ans;
  };

  if (P == 2 or P == 5) {
    cout << solve_naive() << endl;
  } else {
    cout << solve() << endl;
  }
}
