#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template <int sign = 1>
struct Infinity {
  template <typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template <typename T>
  friend bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template <typename T>
  friend bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
const Infinity<> INF;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
  int count;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
void search(const vector<vector<Edge>> &g, int source, int goal, int K,
            i64 init_cost) {
  const int n = g.size();
  auto mincost = vector(n, vector(K + 1, (i64)INF));
  auto pre = vector(n, vector(K + 1, pair(-1, -1)));
  MinHeap<State> que;
  auto push = [&](i64 cost, int node, int count, const State &st) -> bool {
    chmin(count, K);
    if (chmin(mincost[node][count], cost)) {
      pre[node][count] = pair(st.node, st.count);
      que.push({cost, node, count});
      return true;
    }
    return false;
  };
  assert(push(init_cost, source, 1, State{-1, -1, -1}));

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node][cur.count]) continue;
    if (cur.node == goal and cur.count >= K) break;
    for (const auto &e : g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      push(new_cost, e.to, min(cur.count + 1, K), cur);
    }
  }
  if (mincost[goal][K] == (i64)INF) {
    cout << "Impossible" << endl;
    return;
  }
  vector<int> ans;
  int cur = goal, cnt = K;
  while (true) {
    ans.push_back(cur);
    auto [pnode, pcnt] = pre[cur][cnt];
    // if (cur == source and cnt == 1) break;
    if (pnode == -1 and pcnt == -1) break;
    cur = pnode;
    cnt = pcnt;
  }

  reverse(ALL(ans));
  for (auto &x : ans) ++x;
  cout << "Possible" << endl;
  cout << mincost[goal][K] << endl;
  cout << ans.size() << endl;
  print_seq(ans);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  i64 N, S, T, K, M;
  cin >> N >> S >> T >> K;
  --S, --T;
  vector<i64> X(N);
  cin >> X >> M;
  vector<vector<Edge>> g(N);
  REP(i, M) {
    i64 A, B, Y;
    cin >> A >> B >> Y;
    --A, --B;
    g[A].push_back({Y + X[B], (int)B});
  }

  search(g, S, T, K, X[S]);
}
