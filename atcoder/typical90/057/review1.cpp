#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}
#include <boost/dynamic_bitset.hpp>
using BitSet = boost::dynamic_bitset<>;

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

std::vector<std::optional<std::pair<BitSet, int>>> xor_basis(
    const std::vector<BitSet> &a, int m) {
  const int n = (int) a.size();
  std::vector<std::optional<std::pair<BitSet, int>>> basis(m);
  for (int i = 0; i < n; ++i) {
    auto x = a[i];
    for (int j = 0; j < m; ++j) {
      if (not x[j]) continue;
      if (basis[j].has_value()) {
        auto [y, k] = basis[j].value();
        x ^= y;
      } else {
        basis[j] = pair(x, i);
        break;
      }
    }
  }
  return basis;
}

Mint solve() {
  int n = in, m = in;
  vector<BitSet> as(n);
  REP(i, n) {
    int t = in;
    BitSet a(m);
    REP(j, t) {
      int b = in;
      --b;
      a.set(b, true);
    }
    as[i] = std::move(a);
  }

  auto basis = xor_basis(as, m);
  int bcount = 0;
  vector<int> good_bits;
  REP(i, m) {
    if (basis[i].has_value()) {
      good_bits.push_back(i);
      ++bcount;
    }
  }

  vector<int> S = in(m);
  BitSet tmp(m);
  REP(i, m) {
    bool on = S[i] == 1;
    if ((bool) tmp[i] == on) {
      continue;
    }
    if (not basis[i].has_value()) {
      return 0;
    }
    const auto &[b, j] = basis[i].value();
    tmp ^= b;
  }
  return Mint(2).pow(n - bcount);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { print(solve()); }
  exit_();
}
