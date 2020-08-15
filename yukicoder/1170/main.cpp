#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

struct UnionFind {
  vector<int> data;  // positive: parent, negative: size

  UnionFind() = default;

  explicit UnionFind(size_t sz) : data(sz, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    data[y] = x;
    return true;
  }

  int find(int k) {
    if (data[k] < 0) return k;
    return data[k] = find(data[k]);
  }

  int size(int k) { return -data[find(k)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, a, b;
  cin >> n >> a >> b;
  V<int> x(n);
  REP(i, n) cin >> x[i];

  UnionFind uf(n);
  V<int> connected(n + 1);
  REP(i, n) {
    optional<int> l, r;
    auto ub = upper_bound(x.begin(), x.end(), x[i] - a);
    if (ub != x.begin()) {
      --ub;
      int j = ub - x.begin();
      if (x[j] >= x[i] - b) {
        r = j;
      }
    }
    auto lb = lower_bound(x.begin(), x.end(), x[i] - b);
    if (lb != x.end()) {
      int j = lb - x.begin();
      if (x[j] <= x[i] - a) {
        l = j;
      }
    }
    if (l.has_value() && r.has_value()) {
      uf.unite(i, r.value());
      connected[l.value() + 1]++;
      connected[r.value() + 1]--;
    }
  }
  REP(i, n) {
    connected[i + 1] += connected[i];
    if (connected[i + 1] > 0) {
      uf.unite(i, i + 1);
    }
  }

  REP(i, n) { cout << uf.size(i) << '\n'; }
}
