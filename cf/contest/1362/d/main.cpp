//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 n, m;
  cin >> n >> m;
  vector<vector<i64>> adj(n);
  REP(i, m) {
    i64 a, b;
    cin >> a >> b;
    a--;
    b--;
    assert(a != b);
    adj[a].emplace_back(b);
    adj[b].emplace_back(a);
  }
  vector<i64> topic(n);
  for (auto& x : topic) cin >> x;

  int cntr = 0;
  vector<int> ord(n, -1);

  auto solve = [&](auto rec, int cur) -> void {
    i64 t = topic[cur];
    vector<int> neis;
    set<int> covered;
    for (auto nei : adj[cur]) {
      if (topic[nei] == t) {
        cout << "-1" << endl;
        exit(0);
      }
      if (topic[nei] < t) {
        covered.insert(topic[nei]);
      }
    }
    if (covered.size() != t - 1) {
      cout << -1 << endl;
      exit(0);
    }
    if (ord[cur] < 0) {
      ord[cur] = cntr++;
    }
    // for (auto nei : neis) {
    //   if (ord[nei] < 0) {
    //     rec(rec, nei);
    //   }
    // }
  };

  vector<tuple<i64, int>> pres(n);
  REP(i, n) { pres[i] = {topic[i], i}; }
  sort(pres.begin(), pres.end());
  REP(i, n) {
    int j = get<1>(pres[i]);
    if (ord[j] < 0) {
      solve(solve, j);
    }
  }
  assert(cntr == n);

  vector<int> ans(n);
  REP(i, n) { ans[ord[i]] = i + 1; }
  REP(i, n) {
    if (i > 0) cout << ' ';
    cout << ans[i];
  }
  cout << endl;
}
