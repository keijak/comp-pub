#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using namespace std;

const i64 BIG = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
auto search(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();
  vector mincost(n, BIG);
  MinHeap<State> que;
  auto push = [&](i64 cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push({cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, source));

  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      push(new_cost, e.to);
    }
  }
  return mincost;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a, b, c;
    cin >> a >> b >> c;
    --a, --b;
    g[a].push_back({c, b});
    g[b].push_back({c, a});
  }
  auto cost1 = search(g, 0);
  auto cost2 = search(g, n - 1);
  REP(i, n) {
    i64 x = cost1[i] + cost2[i];
    cout << x << "\n";
  }
}
