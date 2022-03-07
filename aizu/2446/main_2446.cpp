#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

template<typename T>
void subset_zeta_transform(std::vector<T> &f) {
  const int n = f.size();
  assert((n & (n - 1)) == 0);  // n: power of 2.
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j++) {
      if ((j & i) == 0) {
        f[j | i] += f[j];
      }
    }
  }
}

using i128 = __int128_t;

auto solve() {
  int n = in;
  Int m = in;
  vector<Int> a = in(n);
  vector<int> p = in(n);
  vector<Real> prob(1 << n);
  vector<Int> cont(1 << n);
  REP(bits, 1, 1 << n) {
    Real pr = 1.0;
    vector<Int> b;
    REP(i, n) {
      if (has_bit(bits, i)) {
        pr *= Real(p[i]) / 100.0;
        b.push_back(a[i]);
      } else {
        pr *= Real(100 - p[i]) / 100.0;
      }
    }
    prob[bits] = pr;
    int bsize = ssize(b);
    Int L = b[0];
    REP(j, 1, bsize) {
      Int g = std::gcd(L, b[j]);
      if (i128(L / g) * b[j] > i128(m)) {
        L = m + 1;
        break;
      } else {
        L = (L / g) * b[j];
      }
    }
    int sign = (bsize % 2) ? 1 : -1;
    cont[bits] = sign * (m / L);
  }
  subset_zeta_transform(cont);
  Real ans = 0;
  REP(bits, 1, 1 << n) {
    assert(cont[bits] >= 0);
    ans += prob[bits] * Real(cont[bits]);
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
