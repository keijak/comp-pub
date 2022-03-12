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

int chromatic_number(const vector<uint32_t> &g) {
  uint32_t n = (int) g.size();
  const uint32_t nn = 1 << n;
  int ret = 0;
  for (uint32_t b = 0; b < nn; ++b) {
    [&]() {
      for (uint32_t i = 0; i < n; ++i) {
        if ((b & (1 << i)) and (g[i] & b) != b) return;
      }
      ret = max(ret, __builtin_popcount(b));
    }();
  }
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

  vector<uint32_t> g(n);
  REP(i, n) g[i] |= 1 << i;
  REP(i, n) {
    REP(j, i + 1, n) {
      if (intersects(i, j)) {
        g[i] |= 1 << j;
        g[j] |= 1 << i;
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
