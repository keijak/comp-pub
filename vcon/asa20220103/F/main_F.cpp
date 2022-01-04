#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = std::move(b)), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = std::move(b)), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<bool keep_roots = true>
struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  std::set<int> roots_;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1) {
    if constexpr (keep_roots) {
      for (int i = 0; i < sz; ++i) roots_.insert(i);
    }
  }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    if constexpr (keep_roots) {
      roots_.erase(y);
    }
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  const std::set<int> &roots() const { return roots_; }
};

auto solve() {
  int H = in, W = in;
  vector<string> g(H);
  REP(i, H)cin >> g[i];
  UnionFind<true> uf(H * W);

  auto inside = [&](int x, int y) {
    return 0 <= x and x < H and 0 <= y and y < W;
  };
  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    static const int dx[] = {1, 0, -1, 0, -1, -1, +1, +1};
    static const int dy[] = {0, 1, 0, -1, -1, +1, -1, +1};
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < 8; ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  REP(i, H) {
    REP(j, W) {
      if (g[i][j] != 'o') continue;
      int v = i * W + j;
      for (auto[ni, nj]: adjacent(i, j)) {
        if (g[ni][nj] != 'o') continue;
        int w = ni * W + nj;
        uf.unite(v, w);
      }
    }
  }
  const int s11 = 11 * 11;
  const int s3 = 3 * 3;
  array<int, 3> ans = {0, 0, 0};
  for (int r: uf.roots()) {
    int x = r / W, y = r % W;
    if (g[x][y] != 'o') continue;
    int sz = uf.size(r);
    DUMP(r, sz);
    while (sz % s11 == 0) { sz /= s11; }
    while (sz % s3 == 0) { sz /= s3; }
    if (sz % 11 == 0) {
      ans[2]++;
    } else if (sz % 3 == 0) {
      ans[0]++;
    } else {
      ans[1]++;
    }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print_seq(ans);
  }
}
