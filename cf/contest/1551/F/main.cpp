#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<unsigned M>
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
    _v = ((unsigned long long) (_v) * a._v) % M;
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
    auto[g, x, y] = ext_gcd(_v, M);
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
const unsigned MOD = int(1e9) + 7;
using Mint = ModInt<MOD>;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
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
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// mod: prime
template<class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (nCk)
  T C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

template<typename T, int DMAX>
struct NaiveMult {
  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    const int n = std::min<int>(x.size() + y.size() - 1, DMAX + 1);
    const int mi = std::min<int>(x.size(), n);
    std::vector<T> res(n);
    for (int i = 0; i < mi; ++i) {
      for (int j = 0; j < int(y.size()); ++j) {
        if (i + j >= n) break;
        res[i + j] += x[i] * y[j];
      }
    }
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x) {
    std::vector<T> res(DMAX + 1);
    res[0] = x[0].inv();
    for (int i = 1; i <= DMAX; ++i) {
      T s = 0;
      const int mj = std::min<int>(i + 1, x.size());
      for (int j = 1; j < mj; ++j) {
        s += x[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return res;
  }
};

// Formal Power Series (dense format).
template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static const int DMAX = Mult::dmax();

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int) c.size() <= DMAX + 1);
  }
  DenseFPS(std::initializer_list<T> init) : coeff_(init.begin(), init.end()) {
    assert((int) init.size() <= DMAX + 1);
  }

  DenseFPS(const DenseFPS &other) : coeff_(other.coeff_) {}
  DenseFPS(DenseFPS &&other) : coeff_(std::move(other.coeff_)) {}
  DenseFPS &operator=(const DenseFPS &other) {
    coeff_ = other.coeff_;
    return *this;
  }
  DenseFPS &operator=(DenseFPS &&other) {
    coeff_ = std::move(other.coeff_);
    return *this;
  }

  int size() const { return (int) coeff_.size(); }

  // Returns the coefficient of x^k.
  T operator[](int k) const {
    if (k >= size()) return 0;
    return coeff_[k];
  }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res += scalar;
    return res;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res += y;
    return res;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res -= y;
    return res;
  }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res *= scalar;
    return res;
  }
  friend DenseFPS operator*(const T &scalar, const DenseFPS &y) {
    DenseFPS res = {scalar};
    res *= y;
    return res;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    *this = DenseFPS(Mult::multiply(std::move(this->coeff_), other.coeff_));
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(Mult::multiply(x.coeff_, y.coeff_));
  }

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x : coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res /= scalar;
    return res;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &y) {
    DenseFPS res = {scalar};
    res /= y;
    return res;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    *this *= DenseFPS(Mult::invert(other.coeff_));
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * DenseFPS(Mult::invert(y.coeff_));
  }

  DenseFPS pow(i64 t) const {
    assert(t >= 0);
    DenseFPS res = {1};
    DenseFPS base = *this;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // Divides by (1 - x).
  void cumsum_inplace() {
    for (int i = 1; i < size(); ++i) {
      coeff_[i] += coeff_[i - 1];
    }
  }

  // Divides by (1 - x).
  DenseFPS cumsum() const {
    DenseFPS res = *this;
    res.cumsum_inplace();
    return res;
  }
};

// Formal Power Series (sparse format).
template<typename T>
struct SparseFPS {
  int size_;
  std::vector<int> degree_;
  std::vector<T> coeff_;

  SparseFPS() : size_(0) {}

  explicit SparseFPS(std::vector<std::pair<int, T>> terms)
      : size_(terms.size()), degree_(size_), coeff_(size_) {
    // Sort by degree.
    std::sort(terms.begin(), terms.end(),
              [](const auto &x, const auto &y) { return x.first < y.first; });
    for (int i = 0; i < size_; ++i) {
      auto[d, c] = terms[i];
      assert(d >= 0);
      degree_[i] = d;
      coeff_[i] = c;
    }
  }

  SparseFPS(std::initializer_list<std::pair<int, T>> terms)
      : SparseFPS(std::vector<std::pair<int, T>>(terms.begin(), terms.end())) {}

  inline int size() const { return size_; }
  inline const T &coeff(int i) const { return coeff_[i]; }
  inline int degree(int i) const { return degree_[i]; }
  int max_degree() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int d, T c) {
    assert(d >= 0);
    if (not degree_.empty()) {
      assert(d > degree_.back());
    }
    degree_.push_back(std::move(d));
    coeff_.push_back(std::move(c));
    ++size_;
  }

  // Returns the coefficient of x^d.
  T operator[](int d) const {
    auto it = std::lower_bound(degree_.begin(), degree_.end(), d);
    if (it == degree_.end() or *it != d) return (T) (0);
    int j = std::distance(degree_.begin(), it);
    return coeff_[j];
  }

  SparseFPS &operator+=(const T &scalar) {
    for (auto &x : coeff_) x += scalar;
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &x, const T &scalar) {
    SparseFPS res = x;
    res += scalar;
    return res;
  }
  SparseFPS &operator+=(const SparseFPS &other) {
    *this = this->add(other);
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &x, const SparseFPS &y) {
    return x.add(y);
  }

  SparseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend SparseFPS operator*(const SparseFPS &x, const T &scalar) {
    SparseFPS res = x;
    res *= scalar;
    return res;
  }

  SparseFPS &operator-=(const SparseFPS &other) {
    *this = this->add(other * -1);
    return *this;
  }
  friend SparseFPS operator-(const SparseFPS &x, const SparseFPS &y) {
    return x.add(y * -1);
  }

 private:
  SparseFPS add(const SparseFPS &other) const {
    SparseFPS res;
    int j = 0;  // two pointers (i, j)
    for (int i = 0; i < size(); ++i) {
      const int deg = this->degree(i);
      for (; j < other.size() and other.degree(j) < deg; ++j) {
        res.emplace_back(other.degree(j), other.coeff(j));
      }
      T c = this->coeff(i);
      if (j < other.size() and other.degree(j) == deg) {
        c += other.coeff(j);
        ++j;
      }
      if (c != 0) {
        res.emplace_back(deg, c);
      }
    }
    for (; j < other.size(); ++j) {
      res.emplace_back(other.degree(j), other.coeff(j));
    }
    return res;
  }
};

// Polynomial addition (dense + sparse).
template<typename FPS>
FPS &operator+=(FPS &x, const SparseFPS<typename FPS::T> &y) {
  for (int i = 0; i < y.size(); ++i) {
    if (y.degree(i) > FPS::DMAX) break;  // ignore
    x.coeff_[y.degree(i)] += y.coeff(i);
  }
  return x;
}
template<typename FPS>
FPS operator+(const FPS &x, const SparseFPS<typename FPS::T> &y) {
  auto res = x;
  res += y;
  return res;
}
template<typename FPS>
FPS operator+(const SparseFPS<typename FPS::T> &x, const FPS &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template<typename FPS>
FPS &operator*=(FPS &x, const SparseFPS<typename FPS::T> &y) {
  if (y.size() == 0) {
    return x *= 0;
  }
  typename FPS::T c0 = 0;
  int j0 = 0;
  if (y.degree(0) == 0) {
    c0 = y.coeff(0);
    ++j0;
  }
  const int yd_max = y.degree(y.size() - 1);
  const int xd_max = x.size() - 1;
  const int n = std::min(xd_max + yd_max + 1, FPS::DMAX + 1);
  if (x.size() < n) x.coeff_.resize(n);
  for (int xd = n - 1; xd >= 0; --xd) {
    x.coeff_[xd] *= c0;
    for (int j = j0; j < y.size(); ++j) {
      int yd = y.degree(j);
      if (yd > xd) break;
      x.coeff_[xd] += x[xd - yd] * y.coeff(j);
    }
  }
  return x;
}
template<typename FPS>
FPS operator*(const FPS &x, const SparseFPS<typename FPS::T> &y) {
  auto res = x;
  res *= y;
  return res;
}
template<typename FPS>
FPS operator*(const SparseFPS<typename FPS::T> &x, const FPS &y) {
  return y * x;  // commutative
}

const int D = 105;
using DFPS = DenseFPS<NaiveMult<Mint, D>>;
using SFPS = SparseFPS<Mint>;

auto solve() {
  INPUT(int, n, K);
  vector<vector<int>> g(n);
  vector<array<int, 2>> edges(n - 1);
  REP(i, n - 1) {
    INPUT(int, u, v);
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
    if (u > v) swap(u, v);
    edges[i] = {u, v};
  }
  Factorials fs(n);
  if (K == 2) {
    return fs.C(n, 2);
  }
  auto depth = [&](auto rec, int v, int p, int d, vector<int> &dc) -> void {
    dc[d]++;
    for (auto u : g[v]) {
      if (u == p) continue;
      rec(rec, u, v, d + 1, dc);
    }
  };
  Mint ans = 0;
  REP(v, n) {
    if (ssize(g[v]) <= 1) continue;
    vector<DFPS> dfps(n, DFPS{1});
    for (auto u : g[v]) {
      vector<int> dc(n, 0);
      depth(depth, u, v, 1, dc);
      for (int j = 1; j < n; ++j) {
        dfps[j] *= SFPS{{0, 1}, {1, dc[j]}};
      }
    }
    for (int j = 1; j < n; ++j) {
      ans += dfps[j][K];
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  cin >> t;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
