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
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

struct Person {
  int x, y, t, r;
};

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

using Float = double;

struct Edge {
  Float cost;
  int to;
};

struct State {
  Float cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
std::vector<std::optional<Float>> dijkstra(
    const std::vector<std::vector<Edge>> &g, int source) {
  const int n = g.size();
  std::vector<std::optional<Float>> mincost(n);
  mincost[source] = 0LL;
  MinHeap<State> que;
  que.push({0.0, source});
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (not mincost[cur.node].has_value() or
        cur.cost != mincost[cur.node].value()) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      if (not mincost[e.to].has_value() or mincost[e.to].value() > new_cost) {
        mincost[e.to] = new_cost;
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

double solve() {
  int n;
  cin >> n;
  vector<Person> people(n);
  vector<vector<Edge>> g(n);
  REP(i, n) {
    int x, y, t, r;
    cin >> x >> y >> t >> r;
    people[i] = {x, y, t, r};
  }
  REP(i, n) REP(j, n) {
    if (i == j) continue;
    Float dx = people[i].x - people[j].x;
    Float dy = people[i].y - people[j].y;
    Float dist = std::sqrt(dx * dx + dy * dy);
    Float v = std::min(people[i].t, people[j].r);
    assert(v > 0);
    Float duration = dist / v;
    g[i].push_back({duration, j});
  }
  auto mincost = dijkstra(g, 0);
  assert(ssize(mincost) == n);
  vector<Float> costs(n - 1);
  REP(i, n - 1) {
    assert(mincost[i].has_value());
    costs[i] = mincost[i + 1].value();
  }
  sort(ALL(costs), greater<>());
  Float ans = 0;
  REP(i, n - 1) {
    costs[i] += i;
    chmax(ans, costs[i]);
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  cout << solve() << "\n";
}
