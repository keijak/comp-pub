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
  int N, M;
  cin >> N >> M;
  vector<vector<bool>> G(N, vector<bool>(N));
  REP(i, M) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    G[a][b] = G[b][a] = true;
  }
  vector<int> perm(N);
  REP(i, N) perm[i] = i;
  auto is_ok = [&]() -> bool {
    int v = perm[0];
    if (v != 0) return false;
    for (int j = 1; j < N; ++j) {
      int u = perm[j];
      if (!G[v][u]) return false;
      v = u;
    }
    return true;
  };

  i64 cnt = 0;
  do {
    if (is_ok()) {
      ++cnt;
    }
  } while (next_permutation(perm.begin(), perm.end()));
  cout << cnt << endl;
}