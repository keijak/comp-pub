#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

#include <atcoder/convolution>
#include <atcoder/math>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;
// T: modint
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
      std::vector<T> f(x.begin(), x.begin() + std::min(n, m2));
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

  // Removes trailing zeros.
  void shrink() {
    while (coeff_.size() > 1 and coeff_.back() == T(0)) coeff_.pop_back();
  }

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

  // Multiplies by x^k (with truncation).
  void shift_inplace(int k) {
    if (k > 0) {
      if (size() <= dmax()) {
        coeff_.resize(std::min(size() + k, dmax() + 1), 0);
      }
      for (int i = size() - 1; i >= k; --i) {
        coeff_[i] = coeff_[i - k];
      }
      for (int i = k - 1; i >= 0; --i) {
        coeff_[i] = 0;
      }
    } else if (k < 0) {
      // If coefficients of degrees higher than dmax() were truncated
      // beforehand, you lose the information. Ensure dmax() is big enough.
      coeff_.erase(coeff_.begin(), coeff_.begin() + std::min(-k, size()));
    }
  }
  // Multiplies by x^k.
  DenseFPS shift(int k) const {
    DenseFPS res = *this;
    res.shift_inplace(k);
    return res;
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
    int z = 0;
    const int msz = std::min(size(), other.size());
    while (z < msz and (*this)[z] == T(0) and other[z] == T(0)) ++z;
    if (z == size()) {
      return *this;  // (0/y) == 0 regardless of y.
    }
    if (z == 0) {
      return *this *= DenseFPS(Mult::invert(other.coeff_));
    } else {
      shift_inplace(-z);
      std::vector<T> y(other.coeff_.begin() + std::min(z, other.size()),
                       other.coeff_.end());
      return *this *= DenseFPS(Mult::invert(std::move(y)));
    }
  }
  friend DenseFPS operator/(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) /= g;
  }
};

template<typename FPS, typename T = typename FPS::T>
FPS derivative(const FPS &f) {
  std::vector<T> res(std::max<int>(f.size() - 1, 1));
  for (int i = 1; i < f.size(); ++i) {
    res[i - 1] = f[i] * i;
  }
  return FPS(std::move(res));
}

template<typename FPS, typename T = typename FPS::T>
FPS integral(const FPS &f, T c0 = 0) {
  const int n = int(f.size());
  assert(n >= 1);
  std::vector<T> res(n + 1);
  res[0] = c0;
  res[1] = f[0];
  if (n == 1) return FPS(std::move(res));
  std::vector<T> inv(n + 1);
  inv[1] = 1;
  for (int i = 2; i <= n; ++i) {
    inv[i] = -inv[T::mod() % i] * (T::mod() / i);
  }
  for (int i = 2; i <= n; ++i) {
    res[i] = f.coeff_[i - 1] * inv[i];
  }
  return FPS(std::move(res));
}

template<typename FPS, typename T = typename FPS::T>
FPS exp(FPS f) {
  const int n = int(f.size());
  assert(n > 0 && f[0] == 0);
  std::vector<T> h_drv = std::move(derivative(f).coeff_);
  std::vector<T> g{1}, g_fft{0}, res(std::move(f.coeff_));
  res[0] = 1;
  for (int m = 1; m < n; m <<= 1) {
    const int m2 = m << 1;
    const T m2_inv = Mint(m2).inv();
    std::vector<T> f_fft(res.begin(), res.begin() + m);
    std::vector<T> t = std::move(derivative(FPS(f_fft)).coeff_);
    t.resize(m);
    f_fft.resize(m2);
    atcoder::internal::butterfly(f_fft);

    // Step 2.a'
    if (m > 1) {
      std::vector<T> _f(m);
      for (int i = 0; i < m; ++i) _f[i] = f_fft[i] * g_fft[i];
      atcoder::internal::butterfly_inv(_f);
      _f.erase(_f.begin(), _f.begin() + (m >> 1));
      _f.resize(m);
      atcoder::internal::butterfly(_f);
      for (int i = 0; i < m; ++i) _f[i] *= g_fft[i];
      atcoder::internal::butterfly_inv(_f);
      _f.resize(m / 2);
      T di = (T(-m) * m).inv();
      for (int i = 0, q = m >> 1; i < q; ++i) {
        g.push_back(_f[i] * di);
      }
    }

    // Step 2.b'--d'
    {
      // Step 2.b'
      std::vector<T> r(h_drv.begin(), h_drv.begin() + (m - 1));
      // Step 2.c'
      r.resize(m);
      atcoder::internal::butterfly(r);
      for (int i = 0; i < m; ++i) r[i] *= f_fft[i];
      atcoder::internal::butterfly_inv(r);
      // Step 2.d'
      auto mm = Mint(-m).inv();
      for (int i = 0; i < m; ++i) t[i] += r[i] * mm;
      std::rotate(t.begin(), t.begin() + (m - 1), t.end());
    }

    // Step 2.e'
    t.resize(m2);
    atcoder::internal::butterfly(t);
    g_fft = g;
    g_fft.resize(m2);
    atcoder::internal::butterfly(g_fft);
    for (int i = 0; i < m2; ++i) t[i] *= g_fft[i];
    atcoder::internal::butterfly_inv(t);
    t.resize(m);
    for (auto &x: t) x *= m2_inv;

    // Step 2.f'
    std::vector<T> v(res.begin() + m, res.begin() + min(n, m2));
    v.resize(m);
    t.insert(t.begin(), m - 1, 0);
    t = std::move(integral(FPS(std::move(t))).coeff_);
    for (int i = 0; i < m; ++i) v[i] -= t[m + i];

    // Step 2.g'
    v.resize(m2);
    atcoder::internal::butterfly(v);
    for (int i = 0; i < m2; ++i) v[i] *= f_fft[i];
    atcoder::internal::butterfly_inv(v);
    v.resize(m);
    for (auto &x: v) x *= m2_inv;

    // Step 2.h'
    for (int i = 0, q = min(n - m, m); i < q; ++i) {
      res[m + i] = v[i];
    }
  }
  return FPS(std::move(res));
}

constexpr int D = 500005;
using DF = DenseFPS<NTTMult<Mint, D>>;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  vector<Mint> a(n);
  REP(i, n) {
    int v;
    cin >> v;
    a[i] = v;
  }
  DF f(a);
  DF ef = exp(std::move(f));
  for (int i = 0; i < n; ++i) {
    cout << ef.coeff_[i] << (i == n - 1 ? '\n' : ' ');
  }
}
