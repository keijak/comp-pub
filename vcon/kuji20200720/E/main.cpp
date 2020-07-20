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
  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(3 * n);
  REP(i, m) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    REP(j, 3) {
      int ux = 3 * u + j;
      int vx = 3 * v + (j + 1) % 3;
      g[ux].push_back(vx);
    }
  }
  int vs, vt;
  cin >> vs >> vt;
  vs--;
  vt--;
  vs = 3 * vs;
  vt = 3 * vt;
  auto bfs = [&]() -> i64 {
    deque<tuple<int, i64>> q;
    vector<bool> done(3 * n);
    q.emplace_back(vs, 0);
    done[vs] = true;
    while (q.size()) {
      auto [v, d] = q.front();
      q.pop_front();
      if (v == vt) {
        assert(d % 3 == 0);
        return d / 3;
      }
      for (auto u : g[v]) {
        if (done[u]) continue;
        done[u] = true;
        q.emplace_back(u, d + 1);
      }
    }
    return -1;
  };
  cout << bfs() << endl;
}