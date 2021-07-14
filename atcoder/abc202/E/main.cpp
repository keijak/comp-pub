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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// tour: preorder node ids
// The interval [begin[v], end[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<int> tour;   // (depth)
  vector<int> begin;  // index in the tour on entering the subtree of v
  vector<int> end;    // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()), g(move(g)), begin(n, -1), end(n, -1), depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    begin[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.emplace_back(depth[v]);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
      // tour.emplace_back(depth[v], v);
    }
    end[v] = int(tour.size());
  }
};

void solve() {
  int n;
  cin >> n;
  vector<vector<int>> g(n);
  for (int i = 1; i < n; ++i) {
    int p;
    cin >> p;
    --p;
    g[i].push_back(p);
    g[p].push_back(i);
  }
  EulerTour et(g, 0);
  DUMP(et.tour);

  deque<tuple<int, bool, int, int, int>> events;
  int Q;
  cin >> Q;

  vector<int> ans(Q, -1);
  REP(q, Q) {
    int u, d;
    cin >> u >> d;
    --u;
    int ud = et.depth[u];
    if (d < ud) {
      ans[q] = 0;
      continue;
    }
    if (d == ud) {
      ans[q] = 1;
      continue;
    }
    events.emplace_back(et.begin[u], false, d, et.end[u], q);
    events.emplace_back(et.end[u], true, d, et.begin[u], q);
  }
  sort(ALL(events));

  vector<int> begin_count(Q, -1);
  vector<int> dcount(n, 0);
  for (int i = 0; i <= ssize(et.tour); ++i) {
    while (not events.empty() and get<0>(events.front()) == i) {
      const auto &[pt, ending, qd, other, q] = events.front();
      events.pop_front();
      if (ending) {
        assert(begin_count[q] >= 0);
        i64 res = dcount[qd] - begin_count[q];
        assert(res >= 0);
        ans[q] = res;
      } else {
        begin_count[q] = dcount[qd];
      }
    }

    if (i == ssize(et.tour)) break;

    const int d = et.tour[i];
    dcount[d]++;
  }

  REP(i, Q) {
    assert(ans[i] >= 0);
    cout << ans[i] << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
