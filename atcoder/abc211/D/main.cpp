#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

auto solve() {
  INPUT(int, n, m);
  vector<vector<Edge>> g(n);
  REP(i, m) {
    INPUT(int, a, b);
    --a, --b;
    g[a].push_back({1, b});
    g[b].push_back({1, a});
  }



  // Returns min distance from the start node to each node (if exists).
  auto search_shortest_path = [&](const vector<vector<Edge>> &g, int start, int goal) {
    const int n = g.size();
    auto mincost = vector(n, BIG);
    auto count = vector(n, Mint(0));
    MinHeap<State> que;
    auto push = [&](i64 cost, int node, int from) -> bool {
      if (cost < mincost[node]) {
        if (from != -1) {
          count[node] = count[from];
        }
        mincost[node] = cost;
        que.push(State{cost, node});
        return true;
      } else if (cost == mincost[node]) {
        if (from != -1) {
          count[node] += count[from];
        }
        return true;
      }
      return false;
    };
    count[start] = 1;
    assert(push(0LL, start, -1));

    while (not que.empty()) {
      State cur = que.top();
      que.pop();
      if (cur.cost != mincost[cur.node]) continue;
      if (cur.node == goal) break;
      for (const auto &e : g[cur.node]) {
        auto new_cost = cur.cost + e.cost;
        push(new_cost, e.to, cur.node);
      }
    }
    return count[goal];
  };
  auto res = search_shortest_path(g, 0, n - 1);
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
