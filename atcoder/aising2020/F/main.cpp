#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
  return (int) a.size();
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

#include <atcoder/convolution>
#include <atcoder/math>

// T: modint
template<typename T, int DMAX>
struct ArbitraryModMult {
  using value_type = T;
  static_assert(atcoder::internal::is_modint<T>::value);

  static constexpr int dmax() { return DMAX; }

  static std::vector<T> convolution(const std::vector<T> &x,
                                    const std::vector<T> &y, int size_limit) {
    std::vector<int> xv(x.size());
    std::vector<int> yv(y.size());
    for (int i = 0; i < (int) x.size(); ++i) xv[i] = x[i].val();
    for (int i = 0; i < (int) y.size(); ++i) yv[i] = y[i].val();

    constexpr int M1 = 167772161, M2 = 469762049, M3 = 1224736769;
    const auto z1 = atcoder::convolution<M1>(xv, yv);
    const auto z2 = atcoder::convolution<M2>(xv, yv);
    const auto z3 = atcoder::convolution<M3>(xv, yv);

    const i64 m1_inv_m2 = atcoder::inv_mod(M1, M2);
    const i64 m12_inv_m3 = atcoder::inv_mod(i64(M1) * M2, M3);
    const i64 m12 = i64(M1) * M2 % T::mod();
    const int n = std::min<int>(x.size() + y.size() - 1, size_limit);
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

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    return convolution(x, y, dmax() + 1);
  }

  static std::vector<T> invert(const std::vector<T> &x) {
    assert(x[0].val() != 0);  // must be invertible
    const int n = x.size();
    std::vector<T> res(n);
    res[0] = x[0].inv();
    for (int i = 1; i < n; i <<= 1) {
      const int m = std::min(2 * i, n);
      std::vector<T> f(2 * i), g(2 * i);
      for (int j = 0; j < m; ++j) f[j] = x[j];
      for (int j = 0; j < i; ++j) g[j] = res[j];
      f = convolution(f, g, 2 * i);
      f.resize(2 * i);
      for (int j = 0; j < i; ++j) f[j] = 0;
      f = convolution(f, g, 2 * i);
      for (int j = i; j < m; ++j) res[j] = -f[j];
    }
    return res;
  }
};

// Formal Power Series (dense format).
template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr int dmax() { return Mult::dmax(); }

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1, 0) {}  // = 0 * x^0

  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }
  DenseFPS(std::initializer_list<T> init) : coeff_(init.begin(), init.end()) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
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

  // size <= dmax + 1
  inline int size() const { return static_cast<int>(coeff_.size()); }

  // Returns the coefficient of x^k.
  inline T operator[](int k) const { return (k >= size()) ? 0 : coeff_[k]; }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const T &scalar) {
    return DenseFPS(x) += scalar;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) coeff_.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(x) += y;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) coeff_.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(x) -= y;
  }

  DenseFPS operator-() const { return *this * -1; }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const T &scalar) {
    return DenseFPS(x) *= scalar;
  }
  friend DenseFPS operator*(const T &scalar, const DenseFPS &y) {
    return DenseFPS{scalar} *= y;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    return *this =
               DenseFPS(Mult::multiply(std::move(this->coeff_), other.coeff_));
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(Mult::multiply(x.coeff_, y.coeff_));
  }

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x: coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &x, const T &scalar) {
    return DenseFPS(x) /= scalar;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &y) {
    return DenseFPS{scalar} /= y;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= DenseFPS(Mult::invert(other.coeff_));
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * DenseFPS(Mult::invert(y.coeff_));
  }

  DenseFPS pow(i64 t) const {
    assert(t >= 0);
    DenseFPS res = {1}, base = *this;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // Multiplies by (1 + c * x^k).
  void multiply2_inplace(int k, int c) {
    assert(k > 0);
    if (size() <= dmax()) {
      coeff_.resize(min(size() + k, dmax() + 1), 0);
    }
    for (int i = size() - 1; i >= k; --i) {
      coeff_[i] += coeff_[i - k] * c;
    }
  }
  // Multiplies by (1 + c * x^k).
  DenseFPS multiply2(int k, int c) const {
    DenseFPS res = *this;
    res.multiply2_inplace(k, c);
    return res;
  }

  // Divides by (1 + c * x^k).
  void divide2_inplace(int k, int c) {
    assert(k > 0);
    for (int i = k; i < size(); ++i) {
      coeff_[i] -= coeff_[i - k] * c;
    }
  }
  // Divides by (1 + c * x^k).
  DenseFPS divide2(int k, int c) const {
    DenseFPS res = *this;
    res.divide2_inplace(k, c);
    return res;
  }

  // Multiplies by x^k.
  void shift_inplace(int k) {
    if (k > 0) {
      if (size() <= dmax()) {
        coeff_.resize(min(size() + k, dmax() + 1), 0);
      }
      for (int i = size() - 1; i >= k; --i) {
        coeff_[i] = coeff_[i - k];
      }
      for (int i = k - 1; i >= 0; --i) {
        coeff_[i] = 0;
      }
    } else if (k < 0) {
      k *= -1;
      for (int i = k; i < size(); ++i) {
        coeff_[i - k] = coeff_[i];
      }
      for (int i = size() - k; i < size(); ++i) {
        // If coefficients of degrees higher than dmax() were truncated
        // beforehand, you lose the information. Ensure dmax() is big enough.
        coeff_[i] = 0;
      }
    }
  }
  // Multiplies by x^k.
  DenseFPS shift(int k) const {
    DenseFPS res = *this;
    res.shift_inplace(k);
    return res;
  }

  T eval(const T &a) const {
    T res = 0, x = 1;
    for (auto c: coeff_) {
      res += c * x;
      x *= a;
    }
    return res;
  }
};

constexpr int D = 100000;
using DF = DenseFPS<ArbitraryModMult<Mint, D>>;

Mint bostan_mori(long long k, DF Q, DF P) {
  Mint ret = 0;
  P.coeff_.resize(Q.size() - 1);
  while (k) {
    auto Q2 = Q;
    for (int i = 1; i < Q2.size(); i += 2) Q2.coeff_[i] = -Q2[i];
    auto S = P * Q2;
    auto T = Q * Q2;
    if (k & 1) {
      for (int i = 1; i < S.size(); i += 2) P.coeff_[i >> 1] = S[i];
      for (int i = 0; i < T.size(); i += 2) Q.coeff_[i >> 1] = T[i];
    } else {
      for (int i = 0; i < S.size(); i += 2) P.coeff_[i >> 1] = S[i];
      for (int i = 0; i < T.size(); i += 2) Q.coeff_[i >> 1] = T[i];
    }
    k >>= 1;
  }
  return ret + P[0];
}

Mint f(i64 k) {
  if (k & 1) {
    Mint x = (k + 1) / 2;
    return x * x;
  } else {
    Mint x = k / 2;
    return x * (x + 1);
  }
}

#include <execinfo.h>
#include <unistd.h>

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  DF p = {0, 0, 0, 0, 0, 1};
  DF q = {1, -1};
  q = q.pow(16);
  REP(i, 5) q.multiply2_inplace(1, 1);

  const int T = in;
  REP(t, T) {
    i64 n = in;
    print(bostan_mori(n, q, p));
  }
}
