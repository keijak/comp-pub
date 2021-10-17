#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
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

struct VersatileInput {
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;
template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      static_assert(std::numeric_limits<T>::max() != T());  // max must be defined
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

struct Edge {
  int to;
  i64 sum;
};
bool operator<(const Edge &e1, const Edge &e2) {
  return std::tie(e1.to, e1.sum) < std::tie(e2.to, e2.sum);
}

struct DFSTree {
  int n_;
  int root_;
  vector<vector<Edge>> g_;  // a spanning tree of the original graph.
  vector<int> parent_;
  vector<int> depth_;
  vector<i64> upper_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  set<pair<int, Edge>> back_edges_;

  DFSTree(vector<vector<Edge>> g, int root = 0)
      : n_((int) g.size()),
        root_(root),
        g_(g),
        parent_(n_, -1),
        depth_(n_, -1),
        upper_(n_, (i64) kBig) {
    dfs(root_, -1, 0);
    for (int v = 0; v < n_; ++v) {
      std::sort(g[v].begin(), g[v].end(), [&](const Edge &e1, const Edge &e2) {
        return e1.to < e2.to;
      });
    }
  }
//
//  bool is_connected(int u, int v) const {
//    if (depth_[u] < depth_[v]) std::swap(u, v);
//    return (parent_[u] == v) or back_edges_.count({u, v});
//  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    set<int> non_neighbors;
    for (const auto &e: g_[v]) {
      chmin(upper_[v], e.sum);
      int u = e.to;
      if (u == p) {
        // parent
        continue;
      }
      if (depth_[u] == -1) {
        // child
        dfs(u, v, d + 1);
        continue;
      }
      non_neighbors.insert(u);
      if (depth_[u] < d) {
        // backward
        back_edges_.emplace(v, e);
      } else {
        // forward
      }
    }
    g_[v].erase(std::remove_if(g_[v].begin(), g_[v].end(),
                               [&](const Edge &e) { return non_neighbors.count(e.to); }),
                g_[v].end());
  }
};

auto solve() -> optional<vector<i64>> {
  int n = in, m = in;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a = in, b = in, c = in;
    --a, --b;
    g[a].push_back({b, (i64) c});
    g[b].push_back({a, (i64) c});
  }
  DFSTree dfst(g);

  vector<int> parity(n, -1);
  vector<int> offset(n, (int) kBig);
  parity[0] = 0;
  offset[0] = 0;

  auto dfs = [&](auto &f, int v, int p) -> void {
    for (const auto &e: dfst.g_[v]) {
      int u = e.to;
      if (u == p) continue;
      parity[u] = 1 - parity[v];
      offset[u] = e.sum - offset[v];
      f(f, e.to, v);
    }
  };
  dfs(dfs, 0, -1);
  DUMP(parity);
  DUMP(offset);
  i64 r_lb = 0;
  i64 r_ub = dfst.upper_[0];

  REP(v, n) {
    if (parity[v]) {
      chmax(r_lb, offset[v] - dfst.upper_[v]);
      chmin(r_ub, offset[v]);
    } else {
      chmax(r_lb, -offset[v]);
      chmin(r_ub, dfst.upper_[v] - offset[v]);
    }
  }
  DUMP(r_lb, r_ub);

  for (const auto&[u, e]: dfst.back_edges_) {
    i64 x = e.sum - offset[e.to];
    if (parity[u] != parity[e.to]) {
      if (x != offset[u]) return nullopt;
    } else if (parity[u] == 0) {
      i64 y = x - offset[u];
      if (y & 1) return nullopt;
      y /= 2;
      chmax(r_lb, y);
      chmin(r_ub, y);
    } else {
      i64 y = offset[u] - x;
      if (y & 1) return nullopt;
      y /= 2;
      chmax(r_lb, y);
      chmin(r_ub, y);
    }
  }

  DUMP(r_lb, r_ub);
  if (r_lb > r_ub) return nullopt;

  vector<i64> ans(n);
  REP(v, n) {
    if (parity[v] == 0) {
      ans[v] = offset[v] + r_lb;
    } else {
      ans[v] = offset[v] - r_lb;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    if (not ans) {
      print(-1);
    } else {
      print_seq(ans.value(), "\n");
    }
  }
}
