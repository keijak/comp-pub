// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &output_seq(
    const Container &seq, const char *sep = " ", const char *ends = "\n",
    std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &output_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &output(const T &x) {
  return output_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &output(const T &head, Ts... tail) {
  return output_one(head, ' '), output(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct Diameter {
  int distance;
  std::vector<int> path;
};

// Tree diameter.
// Returns the maximum diameter and two endpoint nodes of the diameter.
Diameter tree_diameter(const std::vector<std::vector<int>> &g) {
  std::vector<int> parent(g.size());

  auto farthest = [&](auto &rec, int v, int p, int d) -> std::pair<int, int> {
    parent[v] = p;
    int rd = d, rv = v;
    for (auto u: g[v]) {
      if (u == p) continue;
      const auto &[child_d, child_v] = rec(rec, u, v, d + 1);
      if (child_d > rd) {
        rd = child_d;
        rv = child_v;
      }
    }
    return {rd, rv};
  };

  const auto &[_, vr] = farthest(farthest, 0, -1, 0);
  const auto &[distance, vl] = farthest(farthest, vr, -1, 0);

  // Restore the path from vl to vr.
  std::vector<int> diameter_path;
  for (int v = vl; v != -1; v = parent[v]) {
    diameter_path.push_back(v);
  }
  assert(diameter_path.back() == vr);
  assert(int(diameter_path.size()) == distance + 1);

  return Diameter{distance, std::move(diameter_path)};
}

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

auto solve() {
  int n = in, K = in;
  vector<vector<int>> g(n);
  vector<pair<int, int>> edges(n - 1);
  REP(i, n - 1) {
    int a = in, b = in;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
    edges[i] = {a, b};
  }

  vector<int> depth(n);

  auto dfs = Rec([&](auto &rec, int v, int p, int d) -> void {
    depth[v] = d;
    for (auto u: g[v]) {
      if (u == p) continue;
      rec(u, v, d + 1);
    }
  });

  int ans = n;
  if (K % 2 == 0) {
    REP(v, n) {
      dfs(v, -1, 0);
      int cnt = 0;
      REP(u, n) {
        if (depth[u] > K / 2) ++cnt;
      }
      chmin(ans, cnt);
    }
  } else {
    REP(i, n - 1) {
      auto [u, v] = edges[i];
      depth.assign(n, -1);
      dfs(u, v, 0);
      dfs(v, u, 0);
      int cnt = 0;
      REP(x, n) {
        if (depth[x] > K / 2) ++cnt;
      }
      chmin(ans, cnt);
    }
  }
  output(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
