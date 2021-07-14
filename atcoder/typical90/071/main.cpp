#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

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

std::mt19937_64 &PRNG() {
  static std::random_device seed_gen;
  static std::mt19937_64 engine(seed_gen());  // non-deterministic
  return engine;
}

// Returns the topogically sorted list of nodes.
// Nodes with zero indegree come first.
// Returns nullopt if the input graph is not a DAG.
// Args:
//   g[a] = {b, c, d}; // directed adjacency list {a->b, a->c, a->d}
optional<vector<int>> topological_sort(const vector<vector<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u : g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) {
      q.emplace_back(v);
    }
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u : g[v]) {
      if (--indeg[u] == 0) {
        q.push_back(u);
      }
    }
  }
  if (int(res.size()) != n) {
    return nullopt;  // not a DAG
  }
  return res;
}

bool solve() {
  int n, m, K;
  cin >> n >> m >> K;
  vector<pair<int, int>> ab(m);
  vector<vector<int>> pre(n), post(n);
  for (auto &[a, b] : ab) {
    cin >> a >> b;
    --a, --b;
    pre[b].push_back(a);
    post[a].push_back(b);
  }
  REP(i, n) {
    sort(ALL(pre[i]));
    sort(ALL(post[i]));
  }

  auto ts1 = topological_sort(post);
  if (not ts1.has_value()) return false;

  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u : post[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) {
      q.emplace_back(v);
    }
  }
  vector<int> res;
  vector<vector<int>> ans;

  auto dfs = [&](auto &dfs, int index) -> void {
    // DUMP(index, v, res, q);
    if (index == n) {
      if (ssize(ans) < K) {
        ans.push_back(res);
      }
      return;
    }
    for (int i = 0; i < min(ssize(q), K); ++i) {
      if (ssize(ans) >= K) return;
      auto w = q[i];
      q.erase(q.begin() + i);
      res.push_back(w + 1);

      vector<int> nex;
      for (auto u : post[w]) {
        if (--indeg[u] == 0) {
          nex.push_back(u);
        }
      }
      for (auto x : nex) {
        q.push_back(x);
      }

      dfs(dfs, index + 1);

      REP(j, nex.size()) { q.pop_back(); }
      for (auto u : post[w]) {
        ++indeg[u];
      }
      res.pop_back();
      q.insert(q.begin() + i, w);
    }
  };

  dfs(dfs, 0);
  if (ssize(ans) < K) return false;
  REP(i, K) { print_seq(ans[i]); }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      cout << -1 << "\n";
    }
  }
}
