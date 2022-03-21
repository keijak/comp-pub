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

// if reshape_graph=true, `g_` is trimmed into a tree.
template<bool reshape_graph = false>
struct DFSTree {
  using Graph = std::vector<std::vector<int>>;

  int n_;
  int root_;
  Graph g_;
  std::vector<int> parent_;
  std::vector<int> depth_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  std::set<std::pair<int, int>> back_edges_;

  explicit DFSTree(Graph g, int root = 0)
      : n_(int(g.size())),
        root_(root),
        g_(std::move(g)),
        parent_(n_, -1),
        depth_(n_, -1) {
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

vector<int> solve() {
  int n = in, m = in;
  vector<vector<int>> g(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  string S = in;
  int root = -1;
  REP(v, n) {
    if (S[v] == '1') {
      root = v;
      break;
    }
  }
  if (root == -1) {
    return {};
  }

  DFSTree<true> dfst(g);
  REP(v, n) {
    DUMP(v, dfst.g_[v]);
  }

  vector<int> path;
  vector<int> cnt(n);

  auto dfs = Rec([&](auto &dfs, int v, int p) -> void {
    path.push_back(v);
    cnt[v]++;
    for (auto u: dfst.g_[v]) {
      if (u == p) continue;
      dfs(u, v);
      path.push_back(v);
      cnt[v]++;
      if ((cnt[u] & 1) != int(S[u] == '1')) {
        path.push_back(u);
        path.push_back(v);
        cnt[u]++;
        cnt[v]++;
      }
    }
  });
  dfs(root, -1);
  if ((cnt[root] & 1) != int(S[root] == '1')) {
    assert(path.back() == root);
    path.pop_back();
  }

  for (auto &x: path) ++x;
  return path;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto path = solve();
    print(path.size());
    print_seq(path);
  }
}
