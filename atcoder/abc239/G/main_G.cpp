#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;
#include <atcoder/maxflow>

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

auto solve() {
  int n = in, m = in;
  vector<pair<int, int>> edges(m);
  vector<vector<pair<int, int>>> g(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    edges[i] = {u, v};
    g[u].push_back({v, i});
    g[v].push_back({u, i});
  }
  vector<Int> C = in(n);
  static constexpr Int kBigCap = 1e15;
  C[0] = C[n - 1] = kBigCap;
  atcoder::mf_graph<Int> mf(m + n * 2 + 2);

  REP(v, n) {
    int v_in = m + 2 * v;
    int v_out = m + 2 * v + 1;
    for (auto u: g[v]) {
      int u_in = m + 2 * u.first;
      mf.add_edge(u.second, u_in, kBigCap);
      mf.add_edge(v_out, u.second, kBigCap);
    }
    if (v != 0 and v != n - 1) {
      mf.add_edge(v_in, v_out, C[v]);
    }
  }
  int kSource = m + 1;
  int kSink = m + 2 * (n - 1);
  auto max_flow = mf.flow(kSource, kSink);
  assert(max_flow < kBigCap);
  auto cut = mf.min_cut(kSource);
  print(max_flow);
  vector<int> ans;
  REP(v, n) {
    int v_in = m + 2 * v;
    int v_out = m + 2 * v + 1;
    if (cut[v_in] != cut[v_out]) {
      ans.push_back(v);
    }
  }
  print(ssize(ans));
  for (auto &v: ans) ++v;
  print_seq(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
