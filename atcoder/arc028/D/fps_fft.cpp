#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/convolution>
#include <atcoder/math>
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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template <typename T, typename... Ts>
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

template <typename T, int DMAX>
struct ArbitraryModMult {
  using value_type = T;
  static_assert(atcoder::internal::is_modint<T>::value);

  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<int> xv(x.size());
    std::vector<int> yv(y.size());
    for (int i = 0; i < (int)x.size(); ++i) xv[i] = x[i].val();
    for (int i = 0; i < (int)y.size(); ++i) yv[i] = y[i].val();

    constexpr int M1 = 167772161, M2 = 469762049, M3 = 1224736769;
    const auto z1 = atcoder::convolution<M1>(xv, yv);
    const auto z2 = atcoder::convolution<M2>(xv, yv);
    const auto z3 = atcoder::convolution<M3>(xv, yv);

    const i64 m1_inv_m2 = atcoder::inv_mod(M1, M2);
    const i64 m12_inv_m3 = atcoder::inv_mod(i64(M1) * M2, M3);
    constexpr i64 m12 = i64(M1) * M2 % T::mod();
    const int n = std::min<int>(x.size() + y.size() - 1, dmax() + 1);
    std::vector<T> res(n);
    for (int i = 0; i < n; ++i) {
      atcoder::static_modint<M2> v1 = z2[i] - z1[i];
      v1 *= m1_inv_m2;
      const i64 w1 = v1.val() * i64(M1);
      atcoder::static_modint<M3> v2 = z3[i] - z1[i] - w1;
      v2 *= m12_inv_m3;
      res[i] = z1[i] + w1 + (v2.val() * m12);
    }
    return res;
  }
};

// Formal Power Series (dense format).
template <typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr int dmax() { return Mult::dmax(); }

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert(int(c.size()) <= dmax() + 1);
  }
  DenseFPS(std::initializer_list<T> init) : coeff_(init.begin(), init.end()) {
    assert(int(init.size()) <= dmax() + 1);
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

  int size() const { return int(coeff_.size()); }

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

  // Divides by (1 - x^k).
  void cumsum_inplace(int k = 1) {
    for (int i = k; i < size(); ++i) {
      coeff_[i] += coeff_[i - k];
    }
  }

  // Divides by (1 - x^k).
  DenseFPS cumsum(int k = 1) const {
    DenseFPS res = *this;
    res.cumsum_inplace();
    return res;
  }

  // Multiplies by (1 - x).
  void diff_inplace() {
    for (int i = 1; i < size(); ++i) {
      coeff_[i] -= coeff_[i - 1];
    }
  }

  // Multiplies by (1 - x).
  DenseFPS diff() const {
    DenseFPS res = *this;
    res.diff_inplace();
    return res;
  }
};

// Formal Power Series (sparse format).
template <typename T>
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
      auto [d, c] = terms[i];
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
    if (it == degree_.end() or *it != d) return (T)(0);
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
template <typename FPS>
FPS &operator+=(FPS &x, const SparseFPS<typename FPS::T> &y) {
  for (int i = 0; i < y.size(); ++i) {
    if (y.degree(i) > FPS::dmax()) break;  // ignore
    x.coeff_[y.degree(i)] += y.coeff(i);
  }
  return x;
}
template <typename FPS>
FPS operator+(const FPS &x, const SparseFPS<typename FPS::T> &y) {
  auto res = x;
  res += y;
  return res;
}
template <typename FPS>
FPS operator+(const SparseFPS<typename FPS::T> &x, const FPS &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template <typename FPS>
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
  const int n = std::min(xd_max + yd_max + 1, FPS::dmax() + 1);
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
template <typename FPS>
FPS operator*(const FPS &x, const SparseFPS<typename FPS::T> &y) {
  auto res = x;
  res *= y;
  return res;
}
template <typename FPS>
FPS operator*(const SparseFPS<typename FPS::T> &x, const FPS &y) {
  return y * x;  // commutative
}

// Polynomial division (dense / sparse).
template <typename FPS>
FPS &operator/=(FPS &x, const SparseFPS<typename FPS::T> &y) {
  assert(y.size() > 0 and y.degree(0) == 0 and y.coeff(0) != 0);
  const auto inv_c0 = y.coeff(0).inv();
  const int yd_max = y.degree(y.size() - 1);
  const int xd_max = x.size() - 1;
  const int n = std::min(xd_max + yd_max + 1, FPS::dmax() + 1);
  if (x.size() < n) x.coeff_.resize(n);
  for (int xd = 0; xd < n; ++xd) {
    for (int j = 1; j < y.size(); ++j) {
      int yd = y.degree(j);
      if (yd > xd) break;
      x.coeff_[xd] -= x[xd - yd] * y[j];
    }
    x.coeff_[xd] *= inv_c0;
  }
  return x;
}
template <typename FPS>
FPS operator/(const FPS &x, const SparseFPS<typename FPS::T> &y) {
  FPS res = x;
  res /= y;
  return res;
}

constexpr int DMAX = 2000;
using DF = DenseFPS<ArbitraryModMult<Mint, DMAX>>;
using SF = SparseFPS<Mint>;

auto solve() {
  INPUT(int, n, m, Q);
  vector<int> a(n);
  cin >> a;
  vector<DF> gl(n + 1, DF{1}), gr(n + 1, DF{1});
  SF d = SF{{0, 1}, {1, -1}};
  REP(i, n) {
    gl[i + 1] = gl[i] * SF{{0, 1}, {a[i] + 1, -1}};
    gl[i + 1] /= d;
  }
  for (int i = n - 1; i >= 0; --i) {
    gr[i] = gr[i + 1] * SF{{0, 1}, {a[i] + 1, -1}};
    gr[i] /= d;
  }
  auto conv = vector(n, DF{1});
  REP(k, n) { conv[k] = gl[k] * gr[k + 1]; }
  REP(qi, Q) {
    INPUT(int, k, x);
    --k;
    print(conv[k][m - x]);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
