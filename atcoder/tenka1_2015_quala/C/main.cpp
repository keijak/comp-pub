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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Bipartite Matching between two sets of vertices
// V1 (size n) and V2 (size m). O(sqrt(n+m) * |E|).
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
      if (~bm.match[i]) {
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
        return (true);
      }
    }
    return (false);
  }
};

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

auto solve() {
  const int m = in, n = in;
  vector A(m, vector(n, 0));
  vector B(m, vector(n, 0));
  REP(i, m) REP(j, n) {
      int x = in;
      A[i][j] = x;
    }
  REP(i, m) REP(j, n) {
      int x = in;
      B[i][j] = x;
    }

  vector<vector<pair<int, int>>> nodes(2);
  int diff_count = 0;
  REP(i, m) REP(j, n) {
      if (A[i][j] != B[i][j]) {
        nodes[A[i][j]].push_back({i, j});
        ++diff_count;
      }
    }
  vector<Compressed<pair<int, int>>> cnodes;
  REP(i, 2) cnodes.emplace_back(nodes[i]);

  HopcroftKarp bm(cnodes[0].size(), cnodes[1].size());

  REP(i, m) REP(j, n - 1) {
      if (A[i][j] != A[i][j + 1] and B[i][j] != B[i][j + 1] and A[i][j] != B[i][j] and A[i][j + 1] != B[i][j + 1]) {
        assert(A[i][j] == B[i][j + 1]);
        assert(A[i][j + 1] == B[i][j]);
        int u = cnodes[A[i][j]].index({i, j});
        int v = cnodes[A[i][j + 1]].index({i, j + 1});
        if (A[i][j] == 1) swap(u, v);
        bm.add_edge(u, v);
      }
    }
  REP(i, m - 1) REP(j, n) {
      if (A[i][j] != A[i + 1][j] and B[i][j] != B[i + 1][j] and A[i][j] != B[i][j] and A[i + 1][j] != B[i + 1][j]) {
        assert(A[i][j] == B[i + 1][j]);
        assert(A[i + 1][j] == B[i][j]);
        int u = cnodes[A[i][j]].index({i, j});
        int v = cnodes[A[i + 1][j]].index({i + 1, j});
        if (A[i][j] == 1) swap(u, v);
        bm.add_edge(u, v);
      }
    }

  const int match_count = bm.bipartite_matching();

  int cost = diff_count - match_count;
  DUMP(cost, diff_count, match_count);
  return cost;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
