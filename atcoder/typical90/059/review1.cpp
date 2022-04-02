#include <bits/stdc++.h>
#include <atcoder/scc>
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
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

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

// Strongly Connected Components
struct SCC {

  explicit SCC(vector<vector<int>> g) {

  }
};

auto solve() {
  int n = in, m = in, Q = in;
  atcoder::scc_graph g(n);
  vector<vector<int>> to(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    g.add_edge(u, v);
    to[u].push_back(v);
  }
  auto scc = g.scc();
  vector<int> scc_id(n);
  vector<vector<int>> sccg(scc.size());
  REP(i, scc.size()) {
    for (auto u: scc[i]) {
      scc_id[u] = i;
    }
  }
  REP(i, scc.size()) {
    for (auto u: scc[i]) {
      for (auto w: to[u]) {
        int j = scc_id[w];
        sccg[j].push_back(i);
      }
    }
  }

  vector<vector<array<int, 2>>> queries(scc.size());
  vector<bool> answers(Q);
  REP(i, Q) {
    int a = in, b = in;
    --a, --b;
    int x = scc_id[a], y = scc_id[b];
    queries[x].push_back({y, i});
  }

  for (int iter = 0; iter < ssize(scc); iter += 64) {
    vector<uint64_t> reachable_from(scc.size());
    REP(i, scc.size()) {
      for (auto j: sccg[i]) {
        reachable_from[i] |= reachable_from[j];
      }
      if (iter <= i and i < iter + 64) {
        reachable_from[i] |= 1ull << (i - iter);
      }
    }
    REP(j, 64) {
      int from_v = iter + j;
      if (from_v >= ssize(scc)) break;
      for (auto[to_v, qi]: queries[from_v]) {
        answers[qi] = has_bit(reachable_from[to_v], j);
      }
    }
  }
  REP(qi, Q) {
    print((bool) answers[qi]);
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit();
}
