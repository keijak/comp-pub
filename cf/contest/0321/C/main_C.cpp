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

struct CentroidTree {
  int n_;
  std::vector<int> cparent;  // parent of each node in the centroid tree
  std::vector<std::vector<int>> cgraph;
  int croot;

  explicit CentroidTree(std::vector<std::vector<int>> g, int root = 0)
      : n_(g.size()), cparent(n_, -1), cgraph(n_), croot(-1) {
    std::vector<std::set<int>> gd(n_);
    std::vector<int> subsize(n_, 0);
    for (int v = 0; v < n_; ++v) {
      gd[v].insert(g[v].begin(), g[v].end());
    }
    build(root, -1, gd, subsize);
    for (int v = 0; v < n_; ++v) {
      const int p = cparent[v];
      if (p == -1) {
        croot = v;
      } else {
        cgraph[p].push_back(v);
        cgraph[v].push_back(p);
      }
    }
  }

 private:
  void build(int v, int p, std::vector<std::set<int>> &gd,
             std::vector<int> &subsize) {
    const int n = dfs_subsize(v, p, gd, subsize);  // fill subsize
    const int c = centroid(v, p, n, gd, subsize);  // find centroid
    cparent[c] = p;

    std::vector<int> tmp(gd[c].begin(), gd[c].end());
    for (auto u: tmp) {
      gd[c].erase(u);
      gd[u].erase(c);
      build(u, c, gd, subsize);
    }
  }

  int dfs_subsize(int v, int p, const std::vector<std::set<int>> &gd,
                  std::vector<int> &subsize) {
    subsize[v] = 1;
    for (auto u: gd[v]) {
      if (u == p) continue;
      subsize[v] += dfs_subsize(u, v, gd, subsize);
    }
    return subsize[v];
  }

  int centroid(int v, int p, int n, const std::vector<std::set<int>> &gd,
               const std::vector<int> &subsize) {
    for (auto u: gd[v]) {
      if (u == p) continue;
      if (subsize[u] > n / 2) {
        return centroid(u, v, n, gd, subsize);
      }
    }
    return v;
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
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  CentroidTree ct(g);

  vector<char> depth(n, -1);
  auto dfs = Rec([&](auto &self, int v, int p, int d) -> bool {
    if (d >= 26) return false;
    depth[v] = char('A' + d);
    for (auto u: ct.cgraph[v]) {
      if (u == p) continue;
      bool ok = self(u, v, d + 1);
      if (not ok) return false;
    }
    return true;
  });
  bool ok = dfs(ct.croot, -1, 0);
  if (not ok) {
    print("Impossible!");
    return;
  }
  print_seq(depth);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    (solve());
  }
}
