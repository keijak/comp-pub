#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
void chmax(T &a, T b) {
  if (a < b) a = std::move(b);
}
template <class T>
void chmin(T &a, T b) {
  if (a > b) a = std::move(b);
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

struct Edge {
  int to;
  int cost;
};

struct State {
  int cost;
  int node;
  int hot_rem;
  int cold_rem;

  bool operator>(const State &other) const { return cost > other.cost; }
};

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, X;
  cin >> N >> M >> X;
  V<int> temp(N);
  cin >> temp;

  V<V<Edge>> g(N);
  REP(i, M) {
    int a, b, d;
    cin >> a >> b >> d;
    a--;
    b--;
    g[a].push_back({b, d});
    g[b].push_back({a, d});
  }

  MinHeap<State> heap;
  heap.push({0, 0, X, 0});
  map<tuple<int, int, int>, int> mincost;
  while (heap.size()) {
    auto state = move(heap.top());
    heap.pop();
    if (state.cost > mincost[{state.node, state.hot_rem, state.cold_rem}]) {
      continue;
    }
    if (state.node == N - 1) {
      cout << state.cost << endl;
      return 0;
    }
    for (const Edge &nex : g[state.node]) {
      int hot_rem = max(state.hot_rem - nex.cost, 0);
      int cold_rem = max(state.cold_rem - nex.cost, 0);
      if (temp[nex.to] == 0) {
        if (cold_rem > 0) continue;
        hot_rem = X;
      }
      if (temp[nex.to] == 2) {
        if (hot_rem > 0) continue;
        cold_rem = X;
      }
      State nstate{state.cost + nex.cost, nex.to, hot_rem, cold_rem};
      tuple<int, int, int> state_tuple{nstate.node, nstate.hot_rem,
                                       nstate.cold_rem};
      auto it = mincost.find(state_tuple);
      if (it == mincost.end() or it->second > nstate.cost) {
        mincost[state_tuple] = nstate.cost;
        heap.push(move(nstate));
      }
    }
  }

  assert(false);
}
