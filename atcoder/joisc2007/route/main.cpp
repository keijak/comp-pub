#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

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
template <typename T>
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
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

#ifdef ENABLE_DEBUG
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

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
  int from;
  //...
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

using P = complex<int>;

i64 dot(const P &p1, const P &p2) {
  return real(p1) * i64(real(p2)) + imag(p1) * i64(imag(p2));
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  V<P> pos(N);
  REP(i, N) {
    int x, y;
    cin >> x >> y;
    pos[i] = {x, y};
  }
  V<V<Edge>> G(N);
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    G[a].push_back({c, b});
    G[b].push_back({c, a});
  }

  const i64 INF = 1e18;
  auto mincost = make_vec(N, N, INF);
  MinHeap<State> heap;
  REP(i, N) { mincost[0][i] = 0; }
  for (const Edge &e : G[0]) {
    mincost[e.to][0] = e.cost;
    heap.push({e.cost, e.to, 0});
  }
  while (not heap.empty()) {
    State cur = move(heap.top());
    heap.pop();
    if (cur.cost > mincost[cur.node][cur.from]) continue;
    if (cur.node == 1) {
      cout << cur.cost << endl;
      return 0;
    }
    for (const auto &e : G[cur.node]) {
      const P &py = pos[cur.node];
      const P &px = pos[cur.from];
      const P &pz = pos[e.to];
      if (dot(px - py, pz - py) > 0) continue;

      i64 new_cost = cur.cost + e.cost;
      if (mincost[e.to][cur.node] > new_cost) {
        mincost[e.to][cur.node] = new_cost;
        heap.push({new_cost, e.to, cur.node});
      }
    }
  }
  cout << -1 << endl;
}
