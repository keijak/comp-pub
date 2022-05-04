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

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

struct Edge {
  int to;
  uint32_t w;
};

auto solve() {
  int n = in, m = in;
  vector<UnionFind> uf;
  REP(i, 30) {
    uf.emplace_back(n);
  }
  vector<vector<bool>> even_reachable(30, vector<bool>(n, false));

  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a = in, b = in;
    --a, --b;
    uint32_t w = in;
    g[a].push_back(Edge{b, w});
    g[b].push_back(Edge{a, w});

    if (w % 2 == 0) {
      REP(j, 30) {
        even_reachable[j][a] = even_reachable[j][b] = true;
      }
    }
    REP(j, 30) {
      if (has_bit(w, j)) {
        uf[j].unite(a, b);
      }
    }
  }
  REP(j, 1, 30) {
    queue<int> q;
    vector<int> visited(n, false);
    REP(v, n) {
      if (even_reachable[j][v]) {
        visited[v] = true;
        q.push(v);
      }
    }
    while (not q.empty()) {
      int v = q.front();
      q.pop();
      for (const auto &e: g[v]) {
        if (not has_bit(e.w, j)) continue;
        if (visited[e.to]) continue;
        visited[e.to] = true;
        even_reachable[j][e.to] = true;
        q.push(e.to);
      }
    }
  }
  vector<bool> even_reachable_any(n, false);
  REP(v, n) {
    REP(j, 1, 30) {
      if (even_reachable[j][v]) {
        even_reachable_any[v] = true;
        break;
      }
    }
  }

  const int Q = in;
  REP(qi, Q) {
    int u = in, v = in;
    --u, --v;
    bool ok = [&]() {
      REP(j, 30) {
        if (uf[j].same(u, v)) {
          return true;
        }
      }
      return false;
    }();

    if (ok) {
      print(0);
    } else if (even_reachable_any[u]) {
      print(1);
    } else {
      print(2);
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
