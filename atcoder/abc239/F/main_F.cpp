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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

// s |= t;
// Set Union.
// The first argument will be updated to the result.
// cf. https://qiita.com/i_saint/items/a8bdce5146bb38e69f72
template<typename S = std::set<int>>
S &operator|=(S &s, const S &t) {
  auto pos = s.begin();
  for (auto it = t.begin(); it != t.end(); ++it, ++pos) {
    pos = s.emplace_hint(pos, *it);
  }
  return s;
}
template<typename S = std::set<int>>
S &operator|=(S &s, S &&t) {
  if (s.size() < t.size()) s.swap(t);  // small-to-large merging
  for (auto pos = s.begin(); !t.empty(); ++pos) {
    pos = s.insert(pos, t.extract(t.begin()));
  }
  return s;
}

vector<int> D;

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size
  vector<set<pair<int, int>, greater<>>> compo_;

  explicit UnionFind(int n) : n_(n), parent_(n, -1), compo_(n) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    compo_[x] |= std::move(compo_[y]);
    compo_[y].clear();
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

bool solve() {
  int n = in, m = in;
  D.resize(n);
  REP(i, n) D[i] = int(in);
  UnionFind uf(n);
  vector<vector<int>> g(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    if (u == v) return false;
    if (not uf.unite(u, v)) return false;
    g[u].push_back(v);
    g[v].push_back(u);
    --D[u], --D[v];
  }
  REP(v, n) {
    if (D[v] < 0) return false;
    int r = uf.find(v);
    if (D[v] > 0) uf.compo_[r].emplace(D[v], v);
  }
  set<tuple<int, int, int, int>, greater<>> pq;
  REP(v, n) {
    if (uf.find(v) != v) continue;
    if (uf.compo_[v].empty()) continue;
    auto it = uf.compo_[v].begin();
    if (it->first > 0)pq.emplace(uf.compo_[v].size(), it->first, it->second, v);
  }

  vector<pair<int, int>> ans;

  for (int ec = m; ec < n - 1; ++ec) {
    DUMP(ec, n - 1, D, ssize(pq));
    DUMP(uf.compo_);
    if (ssize(pq) == 1) break;
    if (ssize(pq) <= 1) return false;
    auto uit = pq.begin();
    if (uit == pq.end()) return false;
    auto[ucsize, ud, u, ru] = *uit;
    DUMP(ud, u, ru);
    if (D[u] <= 0) return false;
    pq.erase(uit);
    uf.compo_[ru].erase(pair{ud, u});
    --D[u];
    if (D[u] >= 1) uf.compo_[ru].insert(pair{D[u], u});

    auto vit = pq.begin();
    if (vit == pq.end()) return false;
    auto[vcsize, vd, v, rv] = *vit;
    DUMP(vd, v, rv);
    if (D[v] <= 0) return false;
    pq.erase(vit);
    uf.compo_[rv].erase(pair{vd, v});
    --D[v];
    if (D[v] >= 1) uf.compo_[rv].insert(pair{D[v], v});
    DUMP(u, v, ud, vd, ru, rv);
    if (u == v or ru == rv) return false;
    ans.emplace_back(u, v);
    uf.unite(u, v);
    if (ec == n - 2) break;

    int r = uf.find(u);
    auto rit = uf.compo_[r].begin();
    DUMP(r, uf.compo_);
    if (rit == uf.compo_[r].end()) return false;
    if (rit->first >= 1) {
      pq.emplace(uf.compo_[r].size(), rit->first, rit->second, r);
    }
  }
  if (ssize(ans) != n - 1 - m) {
    return false;
  }
  if (not all_of(ALL(D), [&](int x) { return x == 0; })) {
    return false;
  }
  for (auto[u, v]: ans) {
    print(u + 1, v + 1);
  }
  return true;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    if (not solve()) {
      print(-1);
    }
  }
  exit_();
}
