#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/convolution>
#include <atcoder/math>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
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
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
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

template<typename T>
T catalan(const Factorials<T> &fs, int k) {
  auto ret = fs.C(2 * k, k);
  if (k > 0) ret -= fs.C(2 * k, k - 1);
  return ret;
}

template<typename T, int DMAX>
struct NTTMult {
  static_assert(atcoder::internal::is_modint<T>::value, "Requires ACL modint.");
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");

  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<T> res = atcoder::convolution(x, y);
    if (int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x, int d = -1) {
    int n = x.size();
    assert(n != 0 && x[0].val() != 0);  // must be invertible
    if (d == -1) d = n;
    assert(d >= 0);
    std::vector<T> res{x[0].inv()};
    for (int m = 1, m2 = 2; m < d; m = m2, m2 *= 2) {
      std::vector<T> f(x.begin(), x.begin() + min(n, m2));
      std::vector<T> g(res);
      f.resize(m2), atcoder::internal::butterfly(f);
      g.resize(m2), atcoder::internal::butterfly(g);
      for (int i = 0; i < m2; ++i) f[i] *= g[i];
      atcoder::internal::butterfly_inv(f);
      f.erase(f.begin(), f.begin() + m);
      f.resize(m2), atcoder::internal::butterfly(f);
      for (int i = 0; i < m2; ++i) f[i] *= g[i];
      atcoder::internal::butterfly_inv(f);
      T iz = T(m2).inv();
      iz *= -iz;
      for (int i = 0; i < m; ++i) f[i] *= iz;
      res.insert(res.end(), f.begin(), f.begin() + m);
    }
    res.resize(d);
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

  DenseFPS() : coeff_(1, 0) {}  // zero

  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }
  DenseFPS(std::initializer_list<T> c) : coeff_(c.begin(), c.end()) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }

  // size <= dmax + 1
  inline int size() const { return static_cast<int>(coeff_.size()); }

  // Returns the coefficient of x^k.
  inline T operator[](int k) const { return (k >= size()) ? 0 : coeff_[k]; }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) += scalar;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) coeff_.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) += g;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) coeff_.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) -= g;
  }

  DenseFPS operator-() const { return *this * -1; }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) *= scalar;
  }
  friend DenseFPS operator*(const T &scalar, const DenseFPS &g) {
    return DenseFPS{scalar} *= g;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    return *this =
               DenseFPS(Mult::multiply(std::move(this->coeff_), other.coeff_));
  }
  friend DenseFPS operator*(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(Mult::multiply(f.coeff_, g.coeff_));
  }

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x: coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) /= scalar;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &g) {
    return DenseFPS{scalar} /= g;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= DenseFPS(Mult::invert(other.coeff_));
  }
  friend DenseFPS operator/(const DenseFPS &f, const DenseFPS &g) {
    return f * DenseFPS(Mult::invert(g.coeff_));
  }

  DenseFPS pow(long long t) const {
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

template<typename FPS, typename T = typename FPS::T>
FPS prefix(const FPS &f, unsigned deg) {
  return FPS(std::vector<T>(f.coeff_.begin(), f.coeff_.begin() + min<unsigned>(deg, f.coeff_.size())));
}

template<typename FPS, typename T = typename FPS::T>
FPS sqrt(const FPS &f) {
  assert(f[0] == T(1));
  T inv2 = T(1) / T(2);
  FPS ss = {1};
  for (unsigned i = 1; i <= FPS::dmax() + 5;) {
    i <<= 1;
    ss = prefix(ss + prefix(f, i) * prefix(FPS{1} / ss, i),
                i) * inv2;
  }
  return ss;
}

constexpr int D = 100005;
using DF = DenseFPS<NTTMult<Mint, D>>;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n = in, m = in;
  vector<int> C = in(n);
  Int mic = *min_element(ALL(C));
  Int mac = *max_element(ALL(C));
  vector<Mint> fv(mac + 1);
  for (auto c: C) {
    fv[c] = 1;
  }
  DF f(fv);
  Factorials fs(2 * m + 2);
  DF g = f;
  DF res;
  for (int i = 1; i <= m; ++i) {
    if (mic * i > m) break;
    res += g * catalan(fs, i);
    g *= f;
  }
  for (int i = 1; i <= m; ++i) {
    print(res[i]);
  }
}
