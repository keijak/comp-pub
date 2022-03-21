#include <bits/stdc++.h>
#include <atcoder/convolution>
#include <atcoder/modint>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;
using Mint = atcoder::static_modint<200003>;
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

constexpr int P = Mint::mod();
constexpr int R = 2;  // primitive root of MOD.

auto solve() {
  int n = in;
  vector<int> a = in(n);
  vector<optional<int>> a2e(P);
  {
    Mint x = 1;
    for (int i = 0; i <= P - 2; ++i) {
      assert (not a2e[x.val()].has_value());
      a2e[x.val()] = i;
      x *= R;
    }
  }
  vector<Int> C(P);
  REP(i, n) {
    if (a[i] == 0) continue;
    assert(a2e[a[i]].has_value());
    int x = a2e[a[i]].value();
    ++C[x];
  }
  auto B = atcoder::convolution_ll(C, C);
  Int ans = 0;
  REP(i, ssize(B)) {
    if (B[i] == 0) continue;
    ans += B[i] * (Mint(R).pow(i).val());
  }
  REP(i, n) {
    if (a[i] == 0) continue;
    int x = a2e[a[i]].value();
    ans -= Mint(R).pow(x * 2).val();
  }
  ans /= 2;
  print(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    (solve());
  }
}
