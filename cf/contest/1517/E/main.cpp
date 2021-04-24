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

template <unsigned M>
struct ModInt {
  constexpr ModInt(long long val = 0) : _v(0) {
    if (val < 0) {
      long long k = (std::abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

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

  constexpr ModInt operator-() const { return ModInt(umod() - _v); }

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

  constexpr ModInt inv() const {
    // Inverse by Extended Euclidean algorithm.
    // M doesn't need to be prime, but x and M must be coprime.
    assert(_v != 0);
    auto [g, x, y] = ext_gcd(_v, M);
    assert(g == 1);  // The GCD must be 1.
    return x;

    // Inverse by Fermat's little theorem. M must be prime.
    //
    //     return pow(M - 2);
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
  // Extended Euclidean algorithm.
  // Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
  static constexpr std::array<long long, 3> ext_gcd(int a, int b) {
    if (b == 0) return {a, 1, 0};
    auto res = ext_gcd(b, a % b);  // = (g, x, y)
    res[1] -= (a / b) * res[2];
    std::swap(res[1], res[2]);
    return res;  // = (g, y, x - (a/b)*y)
  }

  unsigned _v;  // raw value
};
const unsigned MOD = 998244353;
using Mint = ModInt<MOD>;

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

Mint solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;
  const i64 total = accumulate(ALL(a), 0LL);
  const i64 cmax = (total & 1) ? (total / 2) : (total / 2 - 1);  // inclusive
  vector<i64> acum(n + 1, 0), ecum(n + 1, 0), ocum(n + 1, 0);
  REP(i, n) {
    acum[i + 1] = acum[i] + a[i];
    if (i & 1) {
      ecum[i + 1] = ecum[i];
      ocum[i + 1] = ocum[i] + a[i];
    } else {
      ecum[i + 1] = ecum[i] + a[i];
      ocum[i + 1] = ocum[i];
    }
  }
  DEBUG(total);
  DEBUG(acum);
  DEBUG(ecum);
  DEBUG(ocum);

  auto midsum = [&](int l, int r) -> i64 {
    if (l & 1) {
      return ecum[r] - ecum[l];
    } else {
      return ocum[r] - ocum[l];
    }
  };

  // l,r even
  Mint ans1 = 0;
  int r = 0;
  for (int l = 0; l < n; l += 2) {
    i64 cs = acum[l];
    if (l + 1 >= n) break;
    if (cs + a[l + 1] > cmax) break;
    r = max(r, l + 2);
    while (r + 2 <= n and cs + midsum(l, r + 2) <= cmax) r += 2;
    ans1 += (r - l) / 2;
  }

  // l,r odd
  Mint ans2 = 0;
  r = 0;
  for (int l = 1; l < n; l += 2) {
    i64 cs = acum[l];
    if (l + 1 >= n) break;
    if (cs + a[l + 1] > cmax) break;
    r = max(r, l + 2);
    while (r + 2 <= n and cs + midsum(l, r + 2) <= cmax) r += 2;
    ans2 += (r - l) / 2;
  }

  // CCCCPPPP
  Mint ans3 = 0;
  {
    int k = 0;
    for (int i = 0; i <= n; ++i) {
      i64 s = acum[i];
      if (s > cmax) break;
      ++k;
    }
    ans3 += k;
  }

  // PPPPPCCCCC
  Mint ans4 = 0;
  {
    int k = 0;
    for (int i = n - 1; i >= 0; --i) {
      i64 s = total - acum[i];
      if (s > cmax) break;
      ++k;
    }
    ans4 += k;
  }

  // P CC P
  Mint ans5 = 0;
  if (n >= 4) {
    int k = 0;
    i64 s = a[1];
    for (int i = 2; i < n - 1; ++i) {
      s += a[i];
      if (s > cmax) break;
      ++k;
    }
    ans5 += k;
  }

  // CCC PP PPC
  Mint ans6 = 0;
  if (n >= 4) {
    int k = 0;
    i64 s = a[n - 1];
    for (int i = 0; i < n - 3; ++i) {
      s += a[i];
      if (s > cmax) break;
      ++k;
    }
    ans6 += k;
  }

  // P CCCPP PC
  Mint ans7 = 0;
  if (n >= 5) {
    int k = 0;
    i64 s = a[n - 1];
    for (int i = 1; i < n - 1; ++i) {
      s += a[i];
      if (s > cmax) break;
      ++k;
    }
    ans7 += k;
  }
  Mint ans = ans1 + ans2 + ans3 + ans4 + ans5 + ans6 + ans7;
  DEBUG(ans1, ans2, ans3, ans4, ans5, ans6, ans7, ans);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) cout << solve() << "\n";
}
