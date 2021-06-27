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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    --num_roots;
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < n; ++i) {
      if (parent[i] < 0) res.push_back(i);
    }
    return res;
  }
};

using namespace std;
constexpr int K = 4;

auto solve() {
  int H, W;
  cin >> H >> W;
  int S = 1;
  REP(i, H) S *= K;

  auto decode = [&](int s) -> vector<int> {
    vector<int> v(H, 0);
    REP(i, H) {
      v[i] = s % K;
      s /= K;
    }
    return v;
  };

  auto encode = [&](const vector<int> &s) -> int {
    int b = 1;
    int ret = 0;
    REP(i, H) {
      ret += s[i] * b;
      b *= K;
    }
    return ret;
  };

  auto is_valid = [&](const vector<int> &v) -> bool {
    bool has1 = false, has2 = false, has3 = false;
    REP(i, H) {
      if (i > 0 and v[i - 1] != 0 and v[i] != 0 and v[i - 1] != v[i]) {
        return false;
      }
      if (v[i] == 1) has1 = true;
      if (v[i] == 2) has2 = true;
      if (v[i] == 3) has3 = true;
    }
    if (not has1) return false;
    if (not has2 and has3) return false;
    return true;
  };

  auto trans = [&](const vector<int> &v1,
                   const vector<int> &bits) -> optional<vector<int>> {
    vector<int> res(H, -1);
    UnionFind uf(H + 3);
    int p1 = H, p2 = H + 1, p3 = H + 2;
    REP(i, 1, H) {
      if (bits[i] and bits[i - 1]) {
        uf.unite(i, i - 1);
      }
    }
    REP(i, H) {
      if (not bits[i]) continue;
      if (v1[i] == 1) {
        uf.unite(i, p1);
      }
      if (v1[i] == 2) {
        uf.unite(i, p2);
      }
      if (v1[i] == 3) {
        uf.unite(i, p3);
      }
    }
    bool has_one = false;
    REP(i, H) {
      if (not bits[i]) {
        res[i] = 0;
        continue;
      }
      if (uf.same(i, p1)) {
        res[i] = 1;
        has_one = true;
      } else if (uf.same(i, p2)) {
        res[i] = -2;
      } else if (uf.same(i, p3)) {
        res[i] = -3;
      }
    }
    if (not has_one) return nullopt;
    map<int, int> color_map;
    int next_color = 2;
    REP(i, H) {
      if (res[i] >= 0) continue;
      int r = uf.find(i);
      auto it = color_map.find(r);
      if (it != color_map.end()) {
        res[i] = it->second;
        continue;
      }
      res[i] = next_color++;
      color_map[r] = res[i];
    }
    return res;
  };

  auto dp = vector(W, vector(S, Mint(0)));
  REP(j, S) {
    if (j % K != 1) continue;  // g[0][0] == black
    auto v1 = decode(j);
    assert(v1[0] == 1);
    if (not is_valid(v1)) continue;
    UnionFind uf(H);
    REP(i, 1, H) if (v1[i - 1] != 0 and v1[i] != 0) {
      assert(v1[i - 1] == v1[i]);
      uf.unite(i, i - 1);
    }
    bool ok = true;
    REP(i, H) REP(j, i) {
      if (v1[i] != 0 and v1[j] == v1[i] and not uf.same(i, j)) {
        ok = false;
      }
    }
    if (not ok) continue;

    DUMP(v1);
    dp[0][j] = 1;
  }
  REP(i, 1, W) {
    REP(j, S) {
      // if (dp[i - 1][j].val() == 0) continue;
      auto v1 = decode(j);
      if (not is_valid(v1)) continue;
      REP(bits, 1 << H) {
        vector<int> bs(H);
        REP(k, H) if (bits >> k & 1) { bs[k] = 1; }
        auto v2 = trans(v1, bs);
        if (not v2) continue;
        int k = encode(v2.value());
        dp[i][k] += dp[i - 1][j];
        DUMP(v1, *v2, dp[i - 1][j]);
      }
    }
  }

  if (W <= 3) {
    REP(i, W) {
      DUMP(i);
      REP(j, S) {
        auto v = decode(j);
        DUMP(v, dp[i][j]);
      }
    }
  }
  Mint ans = 0;
  REP(j, S) {
    auto v = decode(j);
    if (is_valid(v) and v.back() == 1) ans += dp[W - 1][j];
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
