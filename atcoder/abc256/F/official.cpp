// #define NDEBUG
#include <bits/stdc++.h>
#include <atcoder/modint>
#include <atcoder/fenwicktree>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &emit_seq(const Container &seq, const char *sep = " ",
                       const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
inline std::ostream &emit_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &emit(const T &x) { return emit_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &emit(const T &head, Ts... tail) {
  return emit_one(head, ' '), emit(tail...);
}
inline std::ostream &emit() { return std::cout << '\n'; }

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() {
  static const Mint kHalf = Mint(1) / 2;

  const int n = in, Q = in;
  atcoder::fenwick_tree<Mint> a(n), s(n), t(n);
  REP(i, n) {
    Mint x = int(in);
    a.add(i, x);
    s.add(i, Mint(i) * x);
    t.add(i, Mint(i) * i * x);
  }
  REP(_, Q) {
    test_case(_, Q);
    int qtype = in;
    if (qtype == 1) {
      const Int i = int(in) - 1;
      const Mint v = int(in);
      const Mint w = a.sum(i, i + 1);
      a.add(i, v - w);
      s.add(i, i * (v - w));
      t.add(i, i * i * (v - w));
    } else {
      Int x = in;
      Mint res = Mint(x * x + x) * a.sum(0, x) - Mint(2 * x + 1) * s.sum(0, x) + t.sum(0, x);
      emit(res * kHalf);
    }
  }
}

int main() {
  init_();
  solve();
  exit_();
}
