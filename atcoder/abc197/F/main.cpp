#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

const i64 BIG = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  char ch;
};

struct State {
  i64 cost;
  int u;
  int v;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
i64 search(const vector<vector<Edge>> &g) {
  const int n = g.size();
  vector mincost(n, vector(n, BIG));
  MinHeap<State> que;
  auto push = [&](i64 cost, int u, int v) -> bool {
    if (chmin(mincost[u][v], cost)) {
      que.push({cost, u, v});
      return true;
    }
    return false;
  };
  assert(push(0LL, 0, n - 1));

  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.u][cur.v]) {
      continue;
    }
    if (cur.u == cur.v) {
      return cur.cost;
    }
    for (const auto &e1 : g[cur.u]) {
      if (e1.to == cur.v) {
        i64 new_cost = cur.cost + 1;
        push(new_cost, cur.v, cur.v);
      }
      for (const auto &e2 : g[cur.v]) {
        if (e1.ch != e2.ch) continue;
        i64 new_cost = cur.cost + 2;
        push(new_cost, e1.to, e2.to);
      }
    }
  }

  return -1;
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a, b;
    char c;
    cin >> a >> b >> c;
    --a, --b;
    g[a].push_back(Edge{b, c});
    g[b].push_back(Edge{a, c});
  }
  auto res = search(g);
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
