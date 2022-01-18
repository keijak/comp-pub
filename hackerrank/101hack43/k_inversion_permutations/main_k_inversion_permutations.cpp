#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <atcoder/convolution>
#include <atcoder/math>
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}
using Int = long long;

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

    const Int m1_inv_m2 = atcoder::inv_mod(M1, M2);
    const Int m12_inv_m3 = atcoder::inv_mod(Int(M1) * M2, M3);
    const Int m12 = Int(M1) * M2 % T::mod();
    const int n = std::min<int>(x.size() + y.size() - 1, size_limit);
    std::vector<T> res(n);
    for (int i = 0; i < n; ++i) {
      atcoder::static_modint<M2> v1 = z2[i] - z1[i];
      v1 *= m1_inv_m2;
      const Int w1 = v1.val() * Int(M1);
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

  static std::vector<T> invert(const std::vector<T> &x, int sz = -1) {
    const int n = int(x.size());
    assert(n != 0 && x[0] != 0);
    if (sz == -1) sz = dmax() + 1;
    assert(sz > 0);
    std::vector<T> res{x[0].inv()};
    while (int(res.size()) < sz) {
      const int m = res.size();
      const int m2 = m << 1;
      std::vector<T> f(x.begin(), x.begin() + std::min<int>(n, m2));
      f.resize(m2);
      res.resize(m2);
      std::vector<T> s = convolution(f, res, m2);
      s.resize(m2);
      for (int i = 0; i < m2; i++) s[i] = -s[i];
      s[0] += 2;
      res = convolution(s, res, m2);
      res.resize(m2);
    }
    res.resize(sz);
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
  DenseFPS inv(int sz = -1) const {
    return DenseFPS(Mult::invert(coeff_, sz));
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
  if (n >= 2) {
    std::vector<T> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
      auto d = std::div(T::mod(), i);
      inv[i] = -inv[d.rem] * d.quot;
    }
    for (int i = 2; i <= n; ++i) {
      res[i] = f.coeff_[i - 1] * inv[i];
    }
  }
  return FPS(std::move(res));
}

template<typename FPS, typename T = typename FPS::T>
FPS log(const FPS &f, int sz = -1) {
  if (sz == -1) sz = FPS::dmax() + 1;
  FPS g = derivative(f) * f.inv(sz);
  g.coeff_.resize(sz);
  return integral(g);
}

template<typename FPS, typename T = typename FPS::T>
FPS exp(FPS g) {
  int n = g.size();
  FPS ret = {1};
  for (int i = 1; i < n; i <<= 1) {
    FPS f(std::vector<T>(i, 0));
    FPS logr = log(ret, i << 1);
    for (int j = 0; j < i; ++j) {
      f.coeff_[j] += g[i + j] - logr[i + j];
    }
    f *= ret;
    std::copy(f.coeff_.begin(), f.coeff_.begin() + i,
              std::back_inserter(ret.coeff_));
  }
  ret.coeff_.resize(n);
  return ret;
}

template<typename FPS, typename T = typename FPS::T>
FPS product_of_geometric_series(std::vector<int> a) {
  std::sort(a.begin(), a.end());
  const int n = int(a.size());
  const int m = FPS::dmax() + 1;
  FPS nume(std::vector<Mint>(m, 0));
  for (long long k = 1; k < m; ++k) {
    const Mint kinv = T(k).inv();
    for (const auto &e: a) {
      long long t = k * e;
      if (t >= m) break;
      nume.coeff_[t] -= kinv;
    }
  }
  nume.shrink();
  auto deno = T(n) * log(FPS{1, -1});
  return exp(std::move(nume) - std::move(deno));
}

constexpr int D = 100000;
using DF = DenseFPS<ArbitraryModMult<Mint, D>>;
using namespace std;

int main() {
  int n;
  int k;
  cin >> n >> k;

  vector<int> a(n);
  for (int i = 0; i < n; ++i) a[i] = i + 1;
  auto p = product_of_geometric_series<DF>(a);
  cout << p[k] << endl;
  return 0;
}
