#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

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
};

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector a(n, vector(n, 0));
  REP(i, n) { REP(j, n) cin >> a[i][j]; }

  vector<int> salaries(n, 0);
  REP(i, n) { salaries[i] = a[i][i]; }  // leaf salaries

  map<int, int> parent;
  map<int, int> bossid;
  REP(i, n) { bossid[i] = i; }

  vector<tuple<int, int, int>> q;
  REP(i, n) REP(j, i + 1, n) { q.emplace_back(a[i][j], i, j); }
  sort(ALL(q));

  UnionFind uf(n);
  for (auto [sal, i, j] : q) {
    int ri = uf.find(i), rj = uf.find(j);
    int bi = bossid[ri], bj = bossid[rj];
    uf.unite(i, j);
    int rm = uf.find(i);
    if (salaries[bi] == sal and salaries[bj] == sal) {
      continue;
    } else if (salaries[bi] == sal) {
      bossid[rm] = parent[bj] = bi;
    } else if (salaries[bj] == sal) {
      bossid[rm] = parent[bi] = bj;
    } else {
      int new_boss = salaries.size();
      salaries.push_back(sal);
      bossid[rm] = parent[bi] = parent[bj] = new_boss;
    }
  }

  int k = salaries.size();
  cout << k << "\n";
  print_seq(salaries);
  cout << k << "\n";
  for (auto [v, u] : parent) {
    cout << v + 1 << " " << u + 1 << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
