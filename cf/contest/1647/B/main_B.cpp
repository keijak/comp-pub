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
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "YES" : "NO") << endc;
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
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
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

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

auto solve() {
  int n = in, m = in;
  auto g = vector(n, string(m, '0'));
  vector<pair<int, int>> bpos;
  map<pair<int, int>, int> bid;

  REP(i, n) {
    REP(j, m) {
      cin >> g[i][j];
      if (g[i][j] == '1') {
        int id = ssize(bpos);
        bpos.push_back({i, j});
        bid[{i, j}] = id;
      }
    }
  }
  int B = ssize(bpos);

  auto inside = [&](int x, int y) {
    return 0 <= x and x < n and 0 <= y and y < m;
  };

  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    static const int dx[] = {1, 0, -1, 0};
    static const int dy[] = {0, 1, 0, -1};
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < std::size(dx); ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  UnionFind uf(B);
  REP(i, n) {
    REP(j, m) {
      if (g[i][j] != '1') continue;
      int u = bid[{i, j}];
      for (auto[ni, nj]: adjacent(i, j)) {
        if (g[ni][nj] != '1') continue;
        int v = bid[{ni, nj}];
        uf.unite(u, v);
      }
    }
  }

  vector<vector<int>> comp(B);
  REP(v, B) {
    int r = uf.find(v);
    comp[r].push_back(v);
  }
  REP(r, B) {
    if (comp[r].empty()) continue;
    int csize = uf.size(r);
    assert(csize == ssize(comp[r]));
    int mini = kBigVal<int>, maxi = -1;
    int minj = kBigVal<int>, maxj = -1;
    for (auto v: comp[r]) {
      auto[i, j] = bpos[v];
      chmin(mini, i);
      chmax(maxi, i);
      chmin(minj, j);
      chmax(maxj, j);
    }
    if (Int(maxi - mini + 1) * (maxj - minj + 1) != csize) {
      return false;
    }
  }
  return true;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
