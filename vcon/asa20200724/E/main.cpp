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
  vector<vector<tuple<int, i64>>> g(n);
  REP(i, m) {
    i64 l, r, d;
    cin >> l >> r >> d;
    --l;
    --r;
    g[l].emplace_back(r, d);
    g[r].emplace_back(l, -d);
  }
  vector<i64> pos(n);
  vector<bool> done(n);

  auto visit = [&](auto rec, int v) -> bool {
    for (auto [r, d] : g[v]) {
      if (done[r]) {
        if (pos[r] != pos[v] + d) return false;
        continue;
      }
      done[r] = true;
      pos[r] = pos[v] + d;
      if (!rec(rec, r)) return false;
    }
    return true;
  };

  REP(i, n) {
    if (done[i]) continue;
    done[i] = true;
    pos[i] = 0;
    if (!visit(visit, i)) {
      cout << "No" << endl;
      return 0;
    }
  }
  cout << "Yes" << endl;
}
