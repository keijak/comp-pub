#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;
  vector<vector<int>> G(N);
  REP(i, N - 1) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    G[a].push_back(b);
    G[b].push_back(a);
  }
  int root = -1;
  int max_deg = 0;
  REP(i, N) {
    if (max_deg < int(G[i].size())) {
      max_deg = int(G[i].size());
      root = i;
    }
  }
  vector<i64> C(N);
  for (auto& x : C) cin >> x;
  sort(C.rbegin(), C.rend());
  i64 ans = accumulate(C.begin() + 1, C.end(), 0LL);
  cout << ans << endl;

  vector<i64> d(N, -1);
  int k = 0;
  auto dfs = [&](auto rec, int v, int p) {
    if (k == N) return;
    d[v] = C[k++];
    for (auto x : G[v]) {
      if (x == p) continue;
      rec(rec, x, v);
    }
  };
  dfs(dfs, root, -1);
  REP(i, N) { cout << (i == 0 ? "" : " ") << d[i]; }
  cout << endl;
}