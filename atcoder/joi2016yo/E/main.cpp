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

const int INF = 1e9;
const i64 LINF = 1e18;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, K, S;
  cin >> N >> M >> K >> S;
  i64 P, Q;
  cin >> P >> Q;
  V<int> zdist(N, INF);
  MinHeap<tuple<i64, int>> zheap;

  REP(i, K) {
    int c;
    cin >> c;
    c--;
    zdist[c] = 0;
    zheap.emplace(0, c);
  }

  V<V<int>> G(N);
  REP(i, M) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    G[a].push_back(b);
    G[b].push_back(a);
  }

  while (zheap.size()) {
    auto [d, v] = zheap.top();
    zheap.pop();
    d++;
    for (auto u : G[v]) {
      if (d < zdist[u]) {
        zdist[u] = d;
        zheap.emplace(d, u);
      }
    }
  }

  //  REP(i, N) { DEBUG(i + 1, zdist[i]); }

  // Dijkstra
  V<i64> mincost(N, LINF);
  MinHeap<tuple<i64, int>> pq;
  pq.emplace(0, 0);
  mincost[0] = 0;
  while (pq.size()) {
    auto [cost, v] = pq.top();
    pq.pop();
    if (cost > mincost[v]) continue;
    if (v == N - 1) break;

    for (auto u : G[v]) {
      i64 new_cost = cost;
      if (zdist[u] == 0) continue;
      if (u != N - 1) {
        if (zdist[u] <= S) {
          new_cost += Q;  // dangerous
        } else {
          new_cost += P;  // safe
        }
      }
      if (new_cost < mincost[u]) {
        mincost[u] = new_cost;
        pq.emplace(new_cost, u);
      }
    }
  }

  cout << mincost[N - 1] << endl;
}
