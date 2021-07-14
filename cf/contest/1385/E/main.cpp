#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
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

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<tuple<int, bool>>> G(n);
  REP(i, m) {
    int t, x, y;
    cin >> t >> x >> y;
    x--;
    y--;
    if (t == 1) {
      // directed
      G[x].emplace_back(y, true);
    } else {
      // undirected
      G[x].emplace_back(y, false);
      G[y].emplace_back(x, false);
    }
  }

  int counter = 0;
  vector<int> topos(n, -1);
  vector<bool> visited(n, false);
  auto hascycle = [&](auto rec, int v) -> bool {
    if (topos[v] != -1) return false;
    if (visited[v]) return true;
    visited[v] = true;
    for (auto [u, directed] : G[v]) {
      if (!directed) continue;
      if (rec(rec, u)) return true;
    }
    topos[v] = counter++;
    return false;
  };

  REP(i, n) {
    if (topos[i] != -1) continue;
    if (hascycle(hascycle, i)) {
      cout << "NO\n";
      return;
    }
  }
  assert(counter == n);
  REP(i, n) assert(topos[i] != -1);
  cout << "YES\n";
  int ecount = 0;
  REP(i, n) {
    for (auto [j, directed] : G[i]) {
      if (directed) {
        cout << (i + 1) << " " << (j + 1) << "\n";
        ++ecount;
        continue;
      }
      if (i > j) continue;
      if (topos[i] >= topos[j]) {
        cout << (i + 1) << " " << (j + 1) << "\n";
        ++ecount;
      } else {
        cout << (j + 1) << " " << (i + 1) << "\n";
        ++ecount;
      }
    }
  }
  assert(ecount == m);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  cout << flush;
}
