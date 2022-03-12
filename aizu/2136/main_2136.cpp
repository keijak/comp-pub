#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<unsigned M>
struct ModInt {
  constexpr ModInt() : _v(0) {}
  constexpr ModInt(long long val) {
    if (val < 0) {
      long long k = (std::abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  constexpr ModInt operator-() const { return ModInt(umod() - _v); }

  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long) (_v) * a._v) % M;
    return *this;
  }
  constexpr ModInt pow(unsigned long long t) const {
    if (_v == 0) {
      return 0;  // corner case: 0^0 = ?
    }
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // https://qiita.com/Mitarushi/items/8d7fb52e8a80e8008463
  constexpr ModInt inv() const {
    long long b = 1, a = _v;
    while (a > 1) {
      long long q = M / a;
      a = M - a * q;
      b = -b * q % M;
    }
    assert(a == 1);  // if a = 0, _v and M are not coprime.
    if (b < 0) b += M;
    ModInt ret;
    ret._v = (unsigned) b;
    return ret;
  }
  constexpr ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    return ModInt(a) /= b;
  }
  friend constexpr bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend constexpr bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  unsigned _v;  // raw value
};
const unsigned MOD = int(1e9) + 7;
using Mint = ModInt<MOD>;

template<uint32_t mod>
int chromatic_number_internal(const vector<uint32_t> &es, const vector<uint32_t> &ind, int upper) {
  const uint32_t N = es.size();
  const uint32_t NN = 1u << N;
  vector<uint32_t> aux(NN, 1);
  for (int i = 1; i < upper; ++i) {
    uint64_t all = 0;
    for (uint32_t j = 0; j < NN; ++j) {
      uint32_t S = j ^ (j >> 1);
      aux[S] = ((uint64_t) aux[S] * ind[S]) % mod;
      all += (j & 1) ? aux[S] : mod - aux[S];
    }
    if (all % mod) return i;
  }
  return upper;
}

int chromatic_number(const vector<vector<int>> &graph) {
  int N = (int) graph.size();
  vector<uint32_t> es(N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      es[i] |= graph[i][j] << j;
    }
  }
  vector<uint32_t> ind(1 << N);
  ind[0] = 1;
  for (uint32_t S = 1; S < 1 << N; S++) {
    const uint32_t u = __builtin_ctz(S);
    ind[S] = ind[S ^ (1 << u)] + ind[(S ^ (1 << u)) & ~es[u]];
  }
  int ret = N;
  ret = chromatic_number_internal<(int) 1e9 + 7>(es, ind, ret);
  ret = chromatic_number_internal<(int) 1e9 + 11>(es, ind, ret);
  ret = chromatic_number_internal<(int) 1e9 + 21>(es, ind, ret);
  return ret;
}

typedef complex<Real> P;  // Point

const Real EPS = 1e-9;
// inner product: dot(a,b) = |a||b|cosθ
Real dot(P a, P b) { return (conj(a) * b).real(); }
// outer product: cross(a,b) = |a||b|sinθ
Real cross(P a, P b) { return (conj(a) * b).imag(); }

int ccw(P a, P b, P c) {
  b -= a;
  c -= a;
  if (cross(b, c) > EPS) return +1;   // counter clockwise
  if (cross(b, c) < -EPS) return -1;  // clockwise
  if (dot(b, c) < -EPS) return +2;    // c--a--b on line
  if (norm(b) < norm(c)) return -2;   // a--b--c on line or a==b
  return 0;                           // a--c--b on line or a==c or b==c
}
bool isecSS(P a1, P a2, P b1, P b2) {
  return ccw(a1, a2, b1) * ccw(a1, a2, b2) <= 0 &&
      ccw(b1, b2, a1) * ccw(b1, b2, a2) <= 0;
}

auto solve(int n) {
  vector<vector<P>> lines(n);
  REP(i, n) {
    int s = in;
    lines[i].resize(s);
    REP(j, s) {
      int x = in, y = in;
      lines[i][j] = {(Real) x, (Real) y};
    }
  }

  auto intersects = [&](int i, int j) -> bool {
    const auto &si = lines[i];
    const auto &sj = lines[j];
    REP(i, 1, si.size()) {
      REP(j, 1, sj.size()) {
        if (isecSS(si[i - 1], si[i], sj[j - 1], sj[j])) {
          return true;
        }
      }
    }
    return false;
  };

  vector<vector<int>> g(n, vector<int>(n, 0));
  REP(i, n) {
    REP(j, i + 1, n) {
      if (intersects(i, j)) {
        g[i][j] = g[j][i] = true;
      }
    }
  }
  return chromatic_number(g);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  while (true) {
    int n = in;
    if (n == 0) break;
    print(solve(n));
  }
}
