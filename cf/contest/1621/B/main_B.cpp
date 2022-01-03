#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
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

auto solve() {
  int n = in;
  vector<tuple<Int, Int, Int>> segs(n);
  pair<Int, Int> minl = {Int(1e9) + 100, Int(1e9) + 100}, maxr = {-1, 0};
  int il = -1, ir = -1;
  optional<int> j;

  REP(i, n) {
    Int l = in, r = in, c = in;
    segs[i] = {l, r, c};
    if (chmin(minl, pair<Int, Int>{l, c})) {
      il = i;
    }
    if (chmax(maxr, pair<Int, Int>{r, -c})) {
      ir = i;
    }
    assert(il != -1 and ir != -1);
    if (j.has_value()) {
      auto[lj, rj, cj] = segs[*j];
      if (lj != minl.first or rj != maxr.first) {
        j = nullopt;
      }
    }
    if (l == minl.first and r == maxr.first) {
      if (not j.has_value()) {
        j = i;
      } else {
        if (c < get<2>(segs[*j])) {
          j = i;
        }
      }
    }

    Int ans = get<2>(segs[il]);
    if (il != ir) {
      ans += get<2>(segs[ir]);
    }
    if (j.has_value()) {
      Int cj = get<2>(segs[*j]);
      chmin(ans, cj);
    }

    print(ans);
  }
  return Void{};
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
