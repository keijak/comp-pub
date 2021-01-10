#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

using namespace std;

const i64 INF = 1e17;

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<i64> a(n);
  cin >> a;
  i64 ans = -INF;
  vector<vector<int>> g(n);
  REP(i, m) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    g[x].push_back(y);
    chmax(ans, a[y] - a[x]);
  }
  vector<i64> minp(n, INF);
  REP(v, n) {
    chmax(ans, a[v] - minp[v]);
    chmin(minp[v], a[v]);
    for (auto u : g[v]) {
      chmin(minp[u], minp[v]);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
