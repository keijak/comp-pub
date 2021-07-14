#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
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
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const i64 BIG = 1e16;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 time;
  i64 fare;
  int to;
};

struct State {
  i64 time;
  i64 money;
  int node;
};
bool operator>(const State &x, const State &y) { return x.time > y.time; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const vector<vector<Edge>> &g,
                          int start,
                          i64 initial_money,
                          const vector<pair<i64, i64>> &exchanges) {
  const int n = g.size();
  const i64 K = 5050;
  auto mintime = vector(n, vector(K + 1, BIG));
  MinHeap<State> que;
  auto push = [&](i64 time, i64 money, int node) -> bool {
    chmin(money, K);
    if (chmin(mintime[node][money], time)) {
      que.push(State{time, money, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, initial_money, start));

  while (not que.empty()) {
    DUMP(que.size());
    State cur = que.top();
    que.pop();
    if (cur.time != mintime[cur.node][cur.money]) {
      continue;
    }
    {
      auto[charge, duration] = exchanges[cur.node];
      i64 new_money = cur.money + charge;
      i64 new_time = cur.time + duration;
      push(new_time, new_money, cur.node);
    }
    for (const auto &e : g[cur.node]) {
      i64 new_money = cur.money - e.fare;
      if (new_money < 0) continue;
      auto new_time = cur.time + e.time;
      push(new_time, new_money, e.to);
    }
  }
  return mintime;
}

auto solve() {
  INPUT(i64, N, M, S);
  vector<vector<Edge>> g(N);
  REP(i, M) {
    INPUT(int, u, v, a, b);
    --u, --v;
    g[u].push_back(Edge{b, a, v});
    g[v].push_back(Edge{b, a, u});
  }
  vector<pair<i64, i64>> exchanges(N);
  REP(i, N) {
    INPUT(i64, c, d);
    exchanges[i] = {c, d};
  }
  auto mintime = search_shortest_path(g, 0, S, exchanges);
  REP(v, 1, N) {
    auto res = *min_element(ALL(mintime[v]));
    print(res);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    solve();
  }
}
