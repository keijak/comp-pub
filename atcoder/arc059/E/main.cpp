#include <bits/stdc++.h>

#include <atcoder/modint>

#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

// Formal Power Series (dense format).
template <typename T, int DMAX>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int)c.size() <= DMAX + 1);
  }
  DenseFPS(std::initializer_list<T> init) : coeff_(init.begin(), init.end()) {
    assert((int)init.size() <= DMAX + 1);
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

  int size() const { return (int)coeff_.size(); }

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
    *this = this->mul_naive(other);
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return x.mul_naive(y);
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
    return *this *= other.inv_naive();
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * y.inv_naive();
  }

  // Naive inverse. O(N^2)
  DenseFPS inv_naive() const {
    std::vector<T> res(DMAX + 1);
    res[0] = coeff_[0].inv();
    for (int i = 1; i <= DMAX; ++i) {
      T s = 0;
      const int mi = std::min(i + 1, size());
      for (int j = 1; j < mi; ++j) {
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
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; j < other.size(); ++j) {
        if (i + j >= n) break;
        res[i + j] += coeff_[i] * other.coeff_[j];
      }
    }
    return DenseFPS(std::move(res));
  }
};

using namespace std;
using Mint = atcoder::modint1000000007;
ostream &operator<<(ostream &os, const Mint &m) { return os << m.val(); }

const int CMAX = 400;
using FPS = DenseFPS<Mint, CMAX>;

Mint solve() {
  int N, C;
  cin >> N >> C;
  vector<int> A(N), B(N);
  cin >> A >> B;

  FPS f = {1};
  REP(i, N) {
    FPS g = {0};
    for (int x = A[i]; x <= B[i]; ++x) {
      g += 1 / FPS{1, -x};
    }
    f *= g;
  }
  return f[C];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << endl;
}
