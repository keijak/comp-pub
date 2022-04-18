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
using Mint = atcoder::modint1000000007;
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

// mod: prime
template<class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  explicit Factorials(int n) : fact(n + 1), ifact(n + 1) {
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

  // Combination (binomial coefficients)
  T C(Int n, Int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

// Computes the n-th term of a linear recurrence sequence.
template<typename T>
struct Kitamasa {
  const int k;

  // Initial k elements (a[0] .. a[k-1]).
  const std::vector<T> a_;

  // Coefficients of the recurrence.
  //   a[n] = coeff[0]*a[n-1] + coeff[1]*a[n-2] + ... + coeff[k-1]*a[n-k].
  const std::vector<T> coeff_;

  explicit Kitamasa(std::vector<T> a, std::vector<T> c)
      : k(a.size()), a_(a), coeff_(std::move(c)) {}

  // n: 0-indexed.
  T nth(long long n) const {
    assert(n >= 0);
    auto x = nth_coeff(n + 1);
    T res = 0;
    for (int i = 0; i < k; ++i) {
      res += x[i] * a_[i];
    }
    return res;
  }

 private:
  // n: 1-indexed.
  std::vector<T> nth_coeff(long long n) const {
    if (n <= k) {
      std::vector<T> res(k, 0);
      res[n - 1] = 1;
      return res;
    }
    // Doubling.
    if (n & 1) {
      // f(n-1) => f(n)
      return next(nth_coeff(n - 1));
    } else {
      // f(n/2) => f(n)
      std::vector<T> s = nth_coeff(n >> 1);
      std::vector<T> t = s;
      std::vector<T> res(k, 0);
      for (int i = 0; i < k; ++i) {
        t = next(std::move(t));
        for (int j = 0; j < k; ++j) {
          res[j] += t[j] * s[i];
        }
      }
      return res;
    }
  }

  std::vector<T> next(std::vector<T> x) const {
    std::vector<T> res(k, 0);
    for (int i = 1; i < k; ++i) {
      res[i] = x[i - 1];
    }
    for (int i = 0; i < k; ++i) {
      res[i] += x[k - 1] * coeff_[k - 1 - i];
    }
    return res;
  }
};

// Formal Power Series (dense format).
template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr Int dmax() { return Mult::dmax(); }

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
};

template<typename FPS>
typename FPS::T bostan_mori(long long k, FPS P, FPS Q) {
  typename FPS::T ret = 0;
  P.coeff_.resize(Q.size() - 1);
  while (k) {
    auto Q2 = Q;
    for (int i = 1; i < Q2.size(); i += 2) {
      Q2.coeff_[i] = -Q2[i];
    }
    const auto S = P * Q2;
    const auto T = Q * Q2;
    for (int i = int(k & 1); i < S.size(); i += 2) {
      P.coeff_[i >> 1] = S[i];
    }
    for (int i = 0; i < T.size(); i += 2) {
      Q.coeff_[i >> 1] = T[i];
    }
    k >>= 1;
  }
  ret += P[0];
  return ret;
}

template<typename FPS>
Mint linear_recurrence(const vector<Mint> &a, const vector<Mint> &c, long long n) {
  vector<Mint> q(c.size() + 1);
  q[0] = 1;
  for (int i = 0; i < (int) c.size(); ++i) {
    q[i + 1] = -c[i];
  }
  FPS Q(std::move(q));
  FPS P(a);
  P *= Q;
  P.coeff_.resize(Q.size() - 1);
  return bostan_mori(n, P, Q);
}

// T: modint
template<typename T, Int DMAX>
struct ArbitraryModMult {
  using value_type = T;
  static_assert(atcoder::internal::is_modint<T>::value);

  static constexpr Int dmax() { return DMAX; }

  static std::vector<T> convolution(const std::vector<T> &x,
                                    const std::vector<T> &y, Int size_limit) {
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
    const Int n = std::min<Int>(x.size() + y.size() - 1, size_limit);
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
};

using DF = DenseFPS<ArbitraryModMult<Mint, 100005>>;

auto solve() {
  const int n = in;
  print(Mint(2).pow(n) - 1);
  for (int k = 2; k <= n; ++k) {
    vector<Mint> a(k, 2), c(k);
    for (int i = 1; i < k; ++i) {
      a[i] = a[i - 1] + 1;
    }
    c[0] = 1;
    c[k - 1] = 1;
    Mint ans = linear_recurrence<DF>(a, c, n - 1) - 1;
    print(ans);
//    auto kitamasa = Kitamasa<Mint>(a, c);
//    print(kitamasa.nth(n - 1) - 1);
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
  exit_();
}
