#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>

using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

// mod: prime
template <class T = Mint>
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

using namespace std;

// T: modint
template <typename T, int DMAX>
struct NTTMult {
  static_assert(atcoder::internal::is_modint<T>::value, "Requires ACL modint.");
  static_assert(T::mod() == 998244353, "Confirm that the mod is NTT-frindly.");
  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<T> res = atcoder::convolution(x, y);
    if (res.size() > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x) {
    assert(x[0].val() != 0);  // must be invertible
    const int n = x.size();
    std::vector<T> res(n);
    res[0] = x[0].inv();
    for (int i = 1; i < n; i <<= 1) {
      std::vector<T> f(2 * i), g(2 * i);
      for (int j = 0, m = std::min(2 * i, n); j < m; ++j) {
        f[j] = x[j];
      }
      for (int j = 0; j < i; ++j) {
        g[j] = res[j];
      }
      f = atcoder::convolution(f, g);
      f.resize(2 * i);
      for (int j = 0; j < i; ++j) {
        f[j] = 0;
      }
      f = atcoder::convolution(f, g);
      for (int j = i, m = std::min(2 * i, n); j < m; ++j) {
        res[j] = -f[j];
      }
    }
    return res;
  }
};

// Formal Power Series (dense format).
template <typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static const int DMAX = Mult::dmax();

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

const int L = 2000005;
using DFPS = DenseFPS<NTTMult<Mint, L>>;

Mint solve() {
  i64 R, G, B, K, X, Y, Z;
  cin >> R >> G >> B >> K >> X >> Y >> Z;
  const i64 N = R + G + B;
  const i64 bmin = max(K - X, 0LL), rmin = max(K - Y, 0LL),
            gmin = max(K - Z, 0LL);
  const i64 s = rmin + gmin + bmin;
  DUMP(bmin, rmin, gmin, s);
  if (s > K) return Mint(0);

  Factorials fs(N);
  vector<Mint> vr(R + 1), vg(G + 1), vb(B + 1);
  REP(i, rmin, R + 1) { vr[i] = fs.C(R, i); }
  REP(i, gmin, G + 1) { vg[i] = fs.C(G, i); }
  REP(i, bmin, B + 1) { vb[i] = fs.C(B, i); }
  DFPS fr(move(vr)), fg(move(vg)), fb(move(vb));
  auto res = move(fr) * move(fg) * move(fb);
  return res[K];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
