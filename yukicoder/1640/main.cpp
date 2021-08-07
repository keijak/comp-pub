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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct HopcroftKarp {
  vector<vector<int> > graph;
  vector<int> dist, match;
  vector<bool> used, vv;

  HopcroftKarp(int n, int m) : graph(n), match(m, -1), used(n) {}

  void add_edge(int u, int v) {
    graph[u].push_back(v);
  }

  void bfs() {
    dist.assign(graph.size(), -1);
    queue<int> que;
    for (int i = 0; i < (int) graph.size(); i++) {
      if (!used[i]) {
        que.emplace(i);
        dist[i] = 0;
      }
    }

    while (!que.empty()) {
      int a = que.front();
      que.pop();
      for (auto &b : graph[a]) {
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
    for (auto &b : graph[a]) {
      int c = match[b];
      if (c < 0 || (!vv[c] && dist[c] == dist[a] + 1 && dfs(c))) {
        match[b] = a;
        used[a] = true;
        return (true);
      }
    }
    return (false);
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
      if (flow == 0) return (ret);
      ret += flow;
    }
  }

  void output() {
    for (int i = 0; i < (int) match.size(); i++) {
      if (~match[i]) {
        cout << match[i] << "-" << i << endl;
      }
    }
  }
};

auto solve() -> optional<vector<int>> {
  INPUT(int, n);
  HopcroftKarp bm(n, n);
  REP(i, n) {
    INPUT(int, a, b);
    --a, --b;
    bm.add_edge(i, a);
    bm.add_edge(i, b);
  }
  auto matching = bm.bipartite_matching();
  DUMP(matching);
  if (matching != n) return nullopt;

  DUMP(bm.match);
  vector<int> ans(n);
  REP(i, n) {
    ans[bm.match[i]] = i + 1;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  if (not ans) {
    print("No");
  } else {
    print("Yes");
    print_seq(ans.value(), "\n");
  }
}
