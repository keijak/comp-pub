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

struct Edge {
  int to;
  int cost;
};

struct State {
  int node;
  i64 path_cost;
  int move_count;

  State() = default;

  bool operator>(const State &other) const {
    return path_cost > other.path_cost;
  }
};

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  V<int> C(N), R(N);
  REP(i, N) { cin >> C[i] >> R[i]; }
  V<V<int>> G(N);
  REP(i, K) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    G[a].push_back(b);
    G[b].push_back(a);
  }

  MinHeap<State> pq;
  pq.push({0, 0LL, 0});
  //   const i64 INF = 1e18;
  map<pair<int, int>, i64> mincost;

  while (pq.size()) {
    State state = move(pq.top());
    pq.pop();
    {
      auto it = mincost.find({state.node, state.move_count});
      if (it != mincost.end() && state.path_cost > it->second) continue;
    }

    if (state.node == N - 1) {
      cout << state.path_cost << endl;
      return 0;
    }

    for (const auto nex : G[state.node]) {
      if (state.move_count > 0) {
        auto it = mincost.find({nex, state.move_count - 1});
        if (it == mincost.end() || state.path_cost < it->second) {
          mincost[{nex, state.move_count - 1}] = state.path_cost;
          pq.push({nex, state.path_cost, state.move_count - 1});
        }
      }

      {
        int move_count = R[state.node] - 1;
        i64 new_cost = state.path_cost + C[state.node];
        auto it = mincost.find({nex, move_count});
        if (it == mincost.end() || new_cost < it->second) {
          mincost[{nex, move_count}] = new_cost;
          pq.push({nex, new_cost, move_count});
        }
      }
    }
  }
  assert(false);
}