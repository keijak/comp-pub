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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    int ub = std::upper_bound(values.begin(), values.end(), x) - values.begin();
    return ub - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

template<class T>
using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

using Graph = std::vector<std::vector<int>>;

struct State {
  int node;
  Int cost;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start, int goal) {
  const int n = g.size();
  auto mincost = vector(n, kBigVal<Int>);
  queue<State> que;
  auto push = [&](Int cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push(State{node, cost});
      return true;
    }
    return false;
  };
  push(0LL, start);

  while (not que.empty()) {
    State cur = que.front();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + 1;
      push(new_cost, e);
    }
  }
  return mincost[goal];
}

auto solve() -> Int {
  Int H = in, W = in;
  auto inside1 = [&](Int i, Int j) -> bool {
    return 0 <= i and i < H and 0 <= j and j < W;
  };
  int n = in;
  Int sx = in, sy = in;
  Int gx = in, gy = in;
  --sx, --sy, --gx, --gy;
  vector<pair<Int, Int>> obst(n);
  set<pair<Int, Int>> obss;
  vector<Int> xs, ys;
  REP(i, n) {
    Int x = in, y = in;
    --x, --y;
    obst[i] = {x, y};
    obss.insert(obst[i]);
  }
  set<pair<Int, Int>> nodes;
  static const int dx[4] = {1, 0, -1, 0};
  static const int dy[4] = {0, 1, 0, -1};
  REP(i, n) {
    auto[x, y] = obst[i];
    REP(d, 4) {
      Int nx = x + dx[d];
      Int ny = y + dy[d];
      if (inside1(nx, ny) and not obss.count({nx, ny})) {
        nodes.insert({nx, ny});
      }
    }
  }
  if (not nodes.count({gx, gy})) {
    return -1;
  }
  nodes.insert({sx, sy});

  map<pair<Int, Int>, int> node_id;
  for (auto[x, y]: nodes) {
    int id = ssize(node_id);
    node_id[{x, y}] = id;
  }

  map<Int, vector<Int>> xo, yo;
  REP(i, n) {
    auto[x, y] = obst[i];
    xo[x].push_back(y);
    yo[y].push_back(x);
  }
  for (auto &[k, v]: xo) sort(ALL(v));
  for (auto &[k, v]: yo) sort(ALL(v));

  vector<vector<int>> g(ssize(nodes));
  for (auto[pos, v]: node_id) {
    REP(move_y, 2) {
      Int x, y;
      tie(x, y) = pos;
      if (move_y) swap(x, y);
      const auto &xz = (move_y ? xo : yo)[y];
      {
        auto it = lower_bound(ALL(xz), x + 1);
        if (it != xz.end()) {
          Int nx = *it;
          if (nx > x + 1) {
            pair<Int, Int> key = move_y ? pair{y, nx - 1} : pair{nx - 1, y};
            auto jt = node_id.find(key);
            if (jt != node_id.end()) g[v].push_back(jt->second);
          }
        }
      }
      {
        auto it = lower_bound(ALL(xz), x);
        if (it != xz.begin()) {
          --it;
          Int nx = *it;
          if (nx < x - 1) {
            pair<Int, Int> key = move_y ? pair{y, nx + 1} : pair{nx + 1, y};
            auto jt = node_id.find(key);
            if (jt != node_id.end()) {
              if (jt != node_id.end()) g[v].push_back(jt->second);
            }
          }
        }
      }
    }
  }
  int si = node_id[{sx, sy}];
  int gi = node_id[{gx, gy}];
  auto res = search_shortest_path(g, si, gi);
  return (res == kBigVal<Int>) ? -1LL : res;
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
