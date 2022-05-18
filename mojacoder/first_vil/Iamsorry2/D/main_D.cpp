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
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T, Int DMAX>
struct NTTMult {
  static_assert(atcoder::internal::is_modint<T>::value, "Requires ACL modint.");
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");

  using value_type = T;
  static constexpr Int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<T> res = atcoder::convolution(x, y);
    if (Int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x, Int sz = -1) {
    int n = x.size();
    assert(n != 0 && x[0].val() != 0);  // must be invertible
    if (sz == -1) sz = dmax() + 1;
    assert(sz > 0);
    std::vector<T> res{x[0].inv()};
    for (int m = 1, m2 = 2; m < sz; m = m2, m2 *= 2) {
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
    res.resize(sz);
    return res;
  }
};

// Formal Power Series (dense format).
template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr Int dmax() { return Mult::dmax(); }

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff;

  DenseFPS() : coeff(1, 0) {}  // zero

  explicit DenseFPS(std::vector<T> c) : coeff(std::move(c)) {
    while (size() > dmax() + 1) coeff.pop_back();
    assert(size() > 0);
  }
  DenseFPS(std::initializer_list<T> c) : coeff(c.begin(), c.end()) {
    while (size() > dmax() + 1) coeff.pop_back();
    assert(size() > 0);
  }

  // size <= dmax + 1
  inline int size() const { return static_cast<int>(coeff.size()); }

  // Returns the coefficient of x^k.
  inline const T operator[](int k) const {
    return (k >= size()) ? 0 : coeff[k];
  }

  // Removes trailing zeros.
  void shrink() {
    while (coeff.size() > 1 and coeff.back() == T(0)) coeff.pop_back();
  }

  // Convenience method for debug dump.
  std::vector<T> head(int n = 10) const {
    n = min<int>(n, coeff.size());
    return std::vector<T>(coeff.begin(), coeff.begin() + n);
  }

  DenseFPS &operator+=(const T &scalar) {
    coeff[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const T &scalar) {
    return DenseFPS(f) += scalar;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) coeff.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) += g;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) coeff.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) -= g;
  }

  DenseFPS operator-() const { return *this * -1; }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff) x *= scalar;
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
               DenseFPS(Mult::multiply(std::move(this->coeff), other.coeff));
  }
  friend DenseFPS operator*(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(Mult::multiply(f.coeff, g.coeff));
  }

  // Multiplies by x^k (with truncation).
  void shift_inplace(int k) {
    if (k > 0) {
      if (size() <= dmax()) {
        coeff.resize(std::min<Int>(size() + k, dmax() + 1), 0);
      }
      for (int i = size() - 1; i >= k; --i) {
        coeff[i] = coeff[i - k];
      }
      for (int i = k - 1; i >= 0; --i) {
        coeff[i] = 0;
      }
    } else if (k < 0) {
      // If coefficients of degrees higher than dmax() were truncated
      // beforehand, you lose the information. Ensure dmax() is big enough.
      coeff.erase(coeff.begin(), coeff.begin() + std::min(-k, size()));
    }
  }
  // Multiplies by x^k.
  DenseFPS shift(int k) const {
    DenseFPS res = *this;
    res.shift_inplace(k);
    return res;
  }

  DenseFPS inv(int sz = -1) const { return DenseFPS(Mult::invert(coeff, sz)); }

  DenseFPS &operator/=(const T &scalar) {
    T d_inv = T(1) / scalar;
    for (auto &x: coeff) x *= d_inv;
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
      return *this *= DenseFPS(Mult::invert(other.coeff));
    } else {
      shift_inplace(-z);
      std::vector<T> y(other.coeff.begin() + std::min(z, other.size()),
                       other.coeff.end());
      return *this *= DenseFPS(Mult::invert(std::move(y)));
    }
  }
  friend DenseFPS operator/(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(f) /= g;
  }
};

constexpr int D = 200005;
using Poly = DenseFPS<NTTMult<Mint, D>>;

auto solve() {
  int n = in;
  deque<Poly> q;
  for (int i = 1; i <= n; ++i) {
    vector<Mint> v = {Mint(-1) / i, 1};
    q.emplace_back(v);
  }
  while (ssize(q) >= 2) {
    auto p1 = move(q.front());
    q.pop_front();
    auto p2 = move(q.front());
    q.pop_front();
    q.push_back(p1 * p2);
  }
  auto p = q.front();
  vector<Mint> ans;
  for (int i = n; i >= 0; --i) {
    ans.push_back(p[i]);
  }
  print_seq(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
