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

auto solve() -> i64 {
  const int n = in;
  const int K = in;
  vector<i64> C(n);
  cin >> C;
  DUMP(n, K, C);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a = in, b = in;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  if (K == 0) return C[0];

  using S = set<pair<i64, int>>;
  vector<S> ss(n);
  vector<int> parent(n, -1);
  vector<i64> subval(n, -1);
  vector<int> edge_done(n, 0);

  auto dfs = [&](auto &dfs, int v, int p) -> void {
    parent[v] = p;
    S top2;
    for (auto u: g[v]) {
      if (u == p) continue;
      if (edge_done[u]) continue;
      if (subval[u] == -1) dfs(dfs, u, v);
      if (not ss[u].empty()) {
        top2.insert(*ss[u].rbegin());
        while (ssize(top2) > 2) {
          top2.erase(top2.begin());
        }
      }
    }
    if (top2.empty()) {
      top2.insert({C[v], v});
    } else {
      auto it = prev(top2.end());
      auto val = pair{it->first + C[v], it->second};
      top2.erase(it);
      top2.insert(val);
    }
    ss[v] = top2;
    i64 val = 0;
    for (const auto&[psum, leaf]: ss[v]) {
      val += psum;
    }
    subval[v] = val;
  };
  dfs(dfs, 0, -1);

  S pq;
  REP(v, n) {
    pq.insert(pair{subval[v], v});
  }

  REP(v, n) {
    DUMP(v, parent[v], subval[v], ss[v]);
  }

  i64 ans = 0;
  bool root_done = false;
  REP(i, K) {
    DUMP(i, pq);
    if (i == K - 1 and not root_done) {
      ans += subval[0];
      break;
    }
    if (pq.empty()) break;
    auto topi = prev(pq.end());
    int v = topi->second;
    ans += subval[v];
    if (v == 0) {
      root_done = true;
    }
    DUMP(v, subval[v], ss[v]);
    for (const auto&[uval, leaf]: ss[v]) {
      // delete u
      for (int u = leaf; u != v; u = parent[u]) {
        DUMP(u);
        assert(pq.erase({subval[u], u}));
        C[u] = 0;
        edge_done[u] = true;
        dfs(dfs, u, parent[u]);
        pq.insert({subval[u], u});
      }
    }
    C[v] = 0;
    for (int p = v; p != -1; p = parent[p]) {
      DUMP(p);
      assert(pq.erase({subval[p], p}));
      dfs(dfs, p, parent[p]);
      pq.insert({subval[p], p});
      if (edge_done[p]) break;
    }
  }
  return ans;
}

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    DUMP(test_case + 1);
    auto ans = solve();
    cout << "Case #" << (test_case + 1) << ": ";
    print(ans);
  }
}
