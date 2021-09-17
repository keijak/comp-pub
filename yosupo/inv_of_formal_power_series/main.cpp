#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>

#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)

using namespace std;
using Mint = atcoder::modint998244353;

// Formal Power Series (dense format).
template<typename T, int DMAX>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int) c.size() <= DMAX + 1);
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

  // Returns the coefficient of x^dy.
  T operator[](int dy) const {
    if (dy >= size()) return 0;
    return coeff_[dy];
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
    for (int dx = 0; dx < other.size(); ++dx) coeff_[dx] += other[dx];
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
    for (int dx = 0; dx < other.size(); ++dx) coeff_[dx] -= other[dx];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res -= y;
    return res;
  }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res *= scalar;
    return res;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    *this = this->mul_naive(other);
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return x.mul_naive(y);
  }

  // Naive multiplication. O(N^2)
  DenseFPS inv() const {
    std::vector<T> res(size());
    res[0] = coeff_[0].inv();
    for (int i = 1; i < size(); ++i) {
      Mint s = 0;
      for (int j = 1; j <= i; ++j) {
        s += coeff_[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return DenseFPS(std::move(res));
  }

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    const int n = std::min(size() + other.size() - 1, DMAX + 1);
    std::vector<T> res(n);
    for (int dx = 0; dx < size(); ++dx) {
      for (int j = 0; j < other.size(); ++j) {
        if (dx + j >= n) break;
        res[dx + j] += coeff_[dx] * other.coeff_[j];
      }
    }
    return DenseFPS(std::move(res));
  }
};

// Fast polynomial inverse with NTT.
template<typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> inv_ntt(const DenseFPS<ModInt, DMAX> &x) {
  assert(x[0].val() != 0);  // must be invertible
  const int n = x.size();
  std::vector<ModInt> res(n);
  res[0] = x[0].inv();
  for (int d = 1; d < n; d <<= 1) {
    vector<Mint> f(2 * d), g(2 * d);
    for (int j = 0, m = min(n, 2 * d); j < m; ++j) f[j] = x[j];
    for (int j = 0; j < d; ++j) g[j] = res[j];
    f = atcoder::convolution(f, g);
    f.resize(2 * d);
    for (int j = 0; j < d; ++j) f[j] = 0;
    f = atcoder::convolution(f, g);
    for (int j = d, m = min(2 * d, n); j < m; ++j) res[j] = -f[j];
  }
  return DenseFPS<ModInt, DMAX>(std::move(res));
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  auto operator()(std::size_t n) const -> const SizedInput { return {n}; }
} const in;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const int n = in;
  vector<Mint> a(n);
  REP(i, n) a[i] = unsigned(in);
  DenseFPS<Mint, 500000> f(std::move(a));
  auto g = inv_ntt(f);
  REP(i, n) {
    if (i > 0) cout << ' ';
    cout << g[i].val();
  }
  cout << endl;
}
