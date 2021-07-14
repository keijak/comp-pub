#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct UnionFind {
  vector<int> data;  // positive: parent, negative: size

  UnionFind() = default;

  explicit UnionFind(size_t sz) : data(sz, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    data[y] = x;
    return true;
  }

  int find(int k) {
    if (data[k] < 0) return k;
    return data[k] = find(data[k]);
  }

  int size(int k) { return -data[find(k)]; }

  bool same(int x, int y) { return find(x) == find(y); }

  vector<int> roots() {
    vector<int> res;
    res.reserve(data.size());
    for (int i = 0; i < (int)data.size(); ++i) {
      if (data[i] < 0) res.push_back(i);
    }
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, C;
  cin >> N >> M >> C;
  V<V<tuple<int, int>>> G(N);

  i64 edge_cost = 0;
  REP(i, M) {
    int a, b, d;
    cin >> a >> b >> d;
    a--;
    b--;
    G[a].emplace_back(b, d);
    G[b].emplace_back(a, d);
    edge_cost += d;
  }

  const i64 INF = 1e18;
  V<i64> mincost(N, INF);
  mincost[0] = 0;

  UnionFind uf(N);
  i64 ans = edge_cost;

  MinHeap<tuple<i64, int, int>> pq;
  pq.emplace(0, 0, -1);
  while (pq.size()) {
    auto [cost, cur, from] = pq.top();
    pq.pop();
    if (cost > mincost[cur]) continue;

    if (from != -1) {
      uf.unite(cur, from);
      for (auto [u, co] : G[cur]) {
        if (uf.same(u, cur)) {
          edge_cost -= co;
        }
      }
      ans = min(ans, cost * C + edge_cost);
    }

    for (auto [nex, co] : G[cur]) {
      i64 new_cost = cost + co;
      if (new_cost < mincost[nex]) {
        mincost[nex] = new_cost;
        pq.emplace(new_cost, nex, cur);
      }
    }
  }

  cout << ans << endl;
}
