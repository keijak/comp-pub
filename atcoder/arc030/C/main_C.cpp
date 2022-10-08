// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
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

// A wrapper class of atcoder::scc_graph.
struct StronglyConnectedComponents {
  template<class T>
  using Vec = std::vector<T>;

  int n;                       // number of vertices in the original graph
  const Vec<Vec<int>> &graph;  // original graph (directed)

  // There may be an edge from components[i] to components[j] if i <= j.
  int scc_n;                 // number of SCCs.
  Vec<Vec<int>> scc_graph;   // scc graph (directed)
  Vec<Vec<int>> components;  // component id -> node list
  Vec<int> component_id;     // node id -> component id

  explicit StronglyConnectedComponents(const Vec<Vec<int>> &g)
      : n(int(g.size())), graph(g) {
    // Compute SCCs.
    atcoder::scc_graph acl_graph(n);
    for (int v = 0; v < n; ++v) {
      for (int u: graph[v]) {
        acl_graph.add_edge(v, u);
      }
    }
    components = acl_graph.scc();
    scc_n = int(components.size());
    component_id.assign(n, -1);
    for (int i = 0; i < scc_n; ++i) {
      for (int v: components[i]) {
        component_id[v] = i;
      }
    }
    // Construct the SCC grpah.
    scc_graph.resize(scc_n);
    for (int i = 0; i < scc_n; ++i) {
      for (int v: components[i]) {
        for (int u: graph[v]) {
          int j = component_id[u];
          if (i != j) scc_graph[i].push_back(j);
        }
      }
    }
    // Uniqify SCC edges.
    for (auto &neighbors: scc_graph) {
      std::sort(neighbors.begin(), neighbors.end());
      neighbors.erase(std::unique(neighbors.begin(), neighbors.end()),
                      neighbors.end());
    }
  }
};

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

auto solve() {
  const int n = in, m = in, K = in;
  vector<char> C = in(n);
  vector<vector<int>> g(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
  }
  StronglyConnectedComponents scc(g);
  auto dp = vector(scc.scc_n, vector<string>());
  optional<string> ans;
  for (int i = scc.scc_n - 1; i >= 0; --i) {
    const auto &vs = scc.components[i];
    int csz = ssize(vs);
    string chars;
    chars.reserve(csz);
    for (int v: vs) {
      chars.push_back(C[v]);
    }
    sort(ALL(chars));
    auto &cdp = dp[i];
    cdp.resize(csz + 1);
    REP(j, 1, csz + 1) {
      cdp[j] = chars.substr(0, j);
    }
    for (int cj: scc.scc_graph[i]) {
      REP(sj, 1, dp[cj].size()) {
        REP(si, csz + 1) {
          if (si + sj > K) break;
          string s = chars.substr(0, si) + dp[cj][sj];
          assert(ssize(s) == si + sj);
          if (ssize(cdp) <= si + sj) {
            cdp.resize(si + sj + 1);
          }
          if (cdp[si + sj].empty()) {
            cdp[si + sj] = s;
          } else {
            chmin(cdp[si + sj], s);
          }
        }
      }
    }
//    REP(j, 1, ssize(cdp)) {
//      DUMP(i, j, cdp[j]);
//    }
    if (ssize(cdp) > K and ssize(cdp[K]) == K) {
      if (not ans) {
        ans = cdp[K];
      } else {
        chmin(ans.value(), cdp[K]);
      }
    }
  }
  if (not ans) {
    out(-1);
  } else {
    out(ans.value());
  }
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
