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

template<bool reshape_graph = false>
struct DFSTree {
  int n_;
  int root_;
  std::vector<int> parent_;
  std::vector<int> depth_;

  // The graph.
  // if reshape_graph=true, `g_` is mutated into a tree.
  std::vector<std::vector<int>> g_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  std::set<std::pair<int, int>> back_edges_;

  explicit DFSTree(std::vector<std::vector<int>> g, int root = 0)
      : n_(int(g.size())),
        root_(root),
        parent_(n_, -1),
        depth_(n_, -1),
        g_(std::move(g)) {
    dfs(root_, -1, 0);
  }

  bool is_connected(int u, int v) const {
    if (depth_[u] < depth_[v]) std::swap(u, v);
    return (parent_[u] == v) or back_edges_.count({u, v});
  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    std::set<int> non_neighbors;
    for (auto u: g_[v]) {
      if (u == p) {  // parent
        continue;
      }
      if (depth_[u] == -1) {  // child
        dfs(u, v, d + 1);
        continue;
      }
      if constexpr (reshape_graph) {
        non_neighbors.insert(u);
      }
      if (depth_[u] < d) {  // backward
        back_edges_.emplace(v, u);
      } else {
        // forward
      }
    }
    if constexpr (reshape_graph) {
      auto &adj = g_[v];
      adj.erase(std::remove_if(adj.begin(), adj.end(),
                               [&](int u) { return non_neighbors.count(u); }),
                adj.end());
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
  int n = in, m = in;
  std::vector<std::vector<int>> g(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  DFSTree<true> tree(g);
  vector<int> outdeg(n, 0);
  set<pair<int, int>> dedges;
  map<int, vector<int>> backmap;
  for (auto[u, v]: tree.back_edges_) {
    backmap[u].push_back(v);
  }

  auto dfs = Rec([&](auto &dfs, int v, int p) -> bool {
    for (auto u: tree.g_[v]) {
      if (u == p) continue;
      dfs(u, v);
    }
    if (outdeg[v] % 2 == 1) {
      if (p == -1) return false;
      outdeg[v]++;
      dedges.insert({v, p});
    } else if (p != -1) {
      outdeg[p]++;
      dedges.insert({p, v});
    }
    for (int u: backmap[v]) {
      outdeg[u]++;
      dedges.insert({u, v});
    }
    return true;
  });
  bool ok = dfs(0, -1);
  if (not ok) {
    print(-1);
  } else {
    for (auto[u, v]: dedges) {
      print(u + 1, v + 1);
    }
  }
  return Void();
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
