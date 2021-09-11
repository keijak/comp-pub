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
  for (auto &x: a) is >> x;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<int sign = 1>
struct Infinity {
  template<typename T>
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
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

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
  int n = in, m = in;
  vector<vector<Edge>> g(n);
  vector<vector<Edge>> bak(n);
  map<pair<int, int>, int> edge_id;
  REP(i, m) {
    int s = in, t = in;
    --s, --t;
    g[s].push_back({1, t});
    bak[t].push_back({1, s});
    edge_id[{s, t}] = i;
  }

  auto search_shortest_path = [&](int start, optional<int> forbidden) -> pair<vector<i64>, vector<int>> {
    vector<i64> mincost(n, i64(kBig));
    vector<int> previous(n, -1);
    queue<State> que;
    auto push = [&](i64 cost, int node, int p) -> bool {
      if (chmin(mincost[node], cost)) {
        previous[node] = p;
        que.push(State{cost, node});
        return true;
      }
      return false;
    };
    assert(push(0LL, start, -1));

    while (not que.empty()) {
      State cur = move(que.front());
      que.pop();
      if (cur.cost > mincost[cur.node]) continue;
      for (const auto &e: g[cur.node]) {
        int ei = edge_id[{cur.node, e.to}];
        if (forbidden.has_value() and forbidden.value() == ei) continue;
        auto new_cost = cur.cost + e.cost;
        push(new_cost, e.to, cur.node);
      }
    }
    return pair{mincost, previous};
  };
  const auto&[mc, previous] = search_shortest_path(0, nullopt);
  DUMP(mc);
  DUMP(previous);
  vector<i64> ans(m, mc[n - 1]);
  int v = n - 1;
  while (v != -1) {
    int p = previous[v];
    if (p == -1) break;
    int e = edge_id[{p, v}];
    const auto&[mc2, pr2] = search_shortest_path(0, e);
    ans[e] = mc2[n - 1];
    v = p;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    for (const auto &x: ans) {
      if (x == kBig) {
        print(-1);
      } else {
        print(x);
      }
    }
  }
}
