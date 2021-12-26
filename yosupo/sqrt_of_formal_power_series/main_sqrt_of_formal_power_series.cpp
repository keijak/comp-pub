#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/math>
#include <atcoder/modint>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
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
};

namespace fps_internal {

std::optional<long long> sqrt_mod(long long a, int p) {
  if (a == 0) return 0;
  if (p == 2) return a;
  if (atcoder::pow_mod(a, (p - 1) >> 1, p) != 1) return std::nullopt;
  long long b = 1;
  while (atcoder::pow_mod(b, (p - 1) >> 1, p) == 1) ++b;
  long long e = 0, m = p - 1;
  while (m % 2 == 0) m >>= 1, ++e;
  auto x = atcoder::pow_mod(a, (m - 1) >> 1, p);
  auto y = a * (x * x % p) % p;
  (x *= a) %= p;
  auto z = atcoder::pow_mod(b, m, p);
  while (y != 1) {
    long long j = 0, t = y;
    while (t != 1) {
      j += 1;
      (t *= t) %= p;
    }
    z = atcoder::pow_mod(z, 1LL << (e - j - 1), p);
    (x *= z) %= p;
    (z *= z) %= p;
    (y *= z) %= p;
    e = j;
  }
  return x;
}

template<typename FPS, typename T = typename FPS::T>
FPS sqrt_fft(const FPS &f_square) {
  assert(f_square[0] == T(1));
  static const T kHalf = T(1) / 2;
  std::vector<T> f{1}, g{1}, z{1};
  T n2_inv = 1;
  for (int n = 1; n <= FPS::dmax(); n *= 2) {
    for (int i = 0; i < n; ++i) z[i] *= z[i];
    atcoder::internal::butterfly_inv(z);
    for (int i = 0; i < n; ++i) z[i] *= n2_inv;

    const int n2 = n * 2;
    n2_inv *= kHalf;

    std::vector<T> delta(n2);
    for (int i = 0; i < n; ++i) delta[n + i] = z[i] - f_square[i] - f_square[n + i];
    atcoder::internal::butterfly(delta);

    std::vector<T> gbuf(n2);
    for (int i = 0; i < n; ++i) gbuf[i] = g[i];
    atcoder::internal::butterfly(gbuf);

    for (int i = 0; i < n2; ++i) delta[i] *= gbuf[i];
    atcoder::internal::butterfly_inv(delta);
    for (int i = 0; i < n2; ++i) delta[i] *= n2_inv;
    f.resize(n2);
    for (int i = n; i < n2; ++i) f[i] = -delta[i] * kHalf;
    if (n2 > FPS::dmax()) break;

    z = f;
    atcoder::internal::butterfly(z);

    std::vector<T> eps = gbuf;
    for (int i = 0; i < n2; ++i) eps[i] *= z[i];
    atcoder::internal::butterfly_inv(eps);
    for (int i = 0; i < n; ++i) eps[i] = 0;
    for (int i = n; i < n2; ++i) eps[i] *= n2_inv;
    atcoder::internal::butterfly(eps);
    for (int i = 0; i < n2; ++i) eps[i] *= gbuf[i];
    atcoder::internal::butterfly_inv(eps);
    for (int i = 0; i < n2; ++i) eps[i] *= n2_inv;
    g.resize(n2);
    for (int i = n; i < n2; ++i) g[i] -= eps[i];
  }
  if ((int) f.size() > FPS::dmax() + 1) {
    f.resize(FPS::dmax() + 1);
  }
  return FPS(std::move(f));
}
}  // namespace fps_internal

template<typename FPS, typename T = typename FPS::T>
std::optional<FPS> fps_sqrt(const FPS &f_square) {
  if (f_square[0].val() == 1) return fps_internal::sqrt_fft(f_square);  // fast path

  int z = 0;
  while (z < f_square.size() && f_square[z].val() == 0) ++z;
  if (z == f_square.size()) return FPS{0};
  if (z % 2 == 1) return std::nullopt;
  const T fz = f_square[z];
  const auto c0 = fps_internal::sqrt_mod(fz.val(), T::mod());
  if (not c0.has_value()) return std::nullopt;
  auto g = FPS(std::vector<T>(f_square.coeff_.begin() + z, f_square.coeff_.end()));
  g = fps_internal::sqrt_fft(g / fz);
  const int zhalf = z / 2;
  const int sz = std::min<int>(g.size() + zhalf, FPS::dmax() + 1);
  std::vector<T> res(sz);
  for (int i = zhalf; i < sz; ++i) res[i] = g[i - zhalf] * (*c0);
  return FPS(std::move(res));
}

constexpr int D = 500000;
using DF = DenseFPS<NTTMult<Mint, D>>;

using namespace std;

int main() {
  int n, v;
  scanf("%d", &n);
  vector<Mint> a(n);
  for (auto &x: a) {
    scanf("%d", &v);
    x = v;
  }
  auto b = fps_sqrt(DF(a));
  if (not b.has_value()) {
    printf("-1\n");
  } else {
    for (int i = 0; i < n; ++i) {
      printf("%u%c", (*b)[i].val(), (i == n - 1 ? '\n' : ' '));
    }
  }
}
