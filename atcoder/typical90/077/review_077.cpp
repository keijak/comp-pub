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

struct HopcroftKarp {
  std::vector<std::vector<int> > graph;
  std::vector<int> dist, match;
  std::vector<bool> used, vv;

  // (n = |V1|, m = |V2|): sizes of the two sets of vertices.
  HopcroftKarp(int n, int m) : graph(n), match(m, -1), used(n) {}

  // Adds an edge between u (in V1) and v (in V2).
  void add_edge(int u, int v) {
    assert(0 <= u and u < (int) graph.size());  // [0, n)
    assert(0 <= v and v < (int) match.size());  // [0, m)
    graph[u].push_back(v);
  }

  // Returns the number of max bipartite matching.
  // The matching result is stored in `match` (match[v] == u).
  int bipartite_matching() {
    int ret = 0;
    while (true) {
      bfs();
      vv.assign(graph.size(), false);
      int flow = 0;
      for (int i = 0; i < (int) graph.size(); i++) {
        if (!used[i] && dfs(i)) ++flow;
      }
      if (flow == 0) return ret;
      ret += flow;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const HopcroftKarp &bm) {
    os << "Matching{";
    for (int i = 0; i < (int) bm.match.size(); i++) {
      if (bm.match[i] != -1) {
        os << "(" << bm.match[i] << ", " << i << "),\n";
      }
    }
    return os << "}\n";
  }

 private:
  void bfs() {
    dist.assign(graph.size(), -1);
    std::queue<int> que;
    for (int i = 0; i < (int) graph.size(); i++) {
      if (!used[i]) {
        que.emplace(i);
        dist[i] = 0;
      }
    }

    while (!que.empty()) {
      int a = que.front();
      que.pop();
      for (auto &b: graph[a]) {
        int c = match[b];
        if (c >= 0 && dist[c] == -1) {
          dist[c] = dist[a] + 1;
          que.emplace(c);
        }
      }
    }
  }

  bool dfs(int a) {
    vv[a] = true;
    for (auto &b: graph[a]) {
      int c = match[b];
      if (c < 0 || (!vv[c] && dist[c] == dist[a] + 1 && dfs(c))) {
        match[b] = a;
        used[a] = true;
        return true;
      }
    }
    return false;
  }
};

auto solve() {
  const int n = in;
  const Int T = in;
  vector<pair<Int, Int>> a(n), b(n);
  REP(i, n) {
    a[i].first = in;
    a[i].second = in;
  }
  map<pair<Int, Int>, int> bmap;
  REP(i, n) {
    b[i].first = in;
    b[i].second = in;
    bmap[b[i]] = i;
  }
  static const Int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
  static const Int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

  HopcroftKarp matcher(n, n);
  REP(i, n) {
    REP(d, 8) {
      auto[x, y] = a[i];
      Int nx = x + dx[d] * T;
      Int ny = y + dy[d] * T;
      auto it = bmap.find({nx, ny});
      if (it != bmap.end()) {
        matcher.add_edge(i, it->second);
      }
    }
  }
  const int m = matcher.bipartite_matching();
  if (m != n) {
    print("No");
    return;
  }
  print("Yes");

  auto get_d = [&](pair<Int, Int> ap, pair<Int, Int> bp) -> optional<int> {
    REP(i, 8) {
      Int nx = ap.first + dx[i] * T;
      Int ny = ap.second + dy[i] * T;
      if (nx == bp.first && ny == bp.second) {
        return i;
      }
    }
    return nullopt;
  };

  vector<int> ans(n);
  REP(i, n) {
    int j = matcher.match[i];
    assert(j >= 0);
    auto d = get_d(a[j], b[i]);
    assert(d.has_value());
    ans[j] = *d + 1;
  }
  print_seq(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
  exit_();
}
