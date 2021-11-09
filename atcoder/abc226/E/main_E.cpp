#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;

set<int> visited;

struct DFSTree {
  int n_;
  int root_;
  vector<vector<int>> g_;  // a spanning tree of the original graph.
  vector<int> parent_;
  vector<int> depth_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  set<pair<int, int>> back_edges_;

  explicit DFSTree(vector<vector<int>> g, int root = 0)
      : n_((int) g.size()), root_(root), g_(g), parent_(n_, -1), depth_(n_, -1) {
    dfs(root_, -1, 0);
  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    visited.insert(v);
//    set<int> non_neighbors;
    for (auto u: g_[v]) {
      if (u == p) {
        // parent
        continue;
      }
      if (depth_[u] == -1) {
        // child
        dfs(u, v, d + 1);
        continue;
      }
//      non_neighbors.insert(u);
      if (depth_[u] < d) {
        // backward
        back_edges_.emplace(v, u);
      } else {
        // forward
      }
    }
//    g_[v].erase(std::remove_if(g_[v].begin(), g_[v].end(),
//                               [&](int u) { return non_neighbors.count(u); }),
//                g_[v].end());
  }
};

auto solve() -> Mint {
  int n = in, m = in;
  vector<vector<int>> g(n);

  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  vector<int> depth(n, -1);
  int back_edges = 0;
  auto dfs = [&](auto &f, int v, int p, int d) -> void {
    depth[v] = d;
    for (auto u: g[v]) {
      if (u == p) continue;
      if (depth[u] == -1) {
        f(f, u, v, d + 1);
      } else if (depth[u] < d) {
        back_edges++;
      }
    }
  };
  Mint ans = 1;
  REP(v, n) {
    if (depth[v] != -1) continue;
    back_edges = 0;
    dfs(dfs, v, -1, 0);
    if (back_edges != 1) return {0};
    ans *= 2;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
