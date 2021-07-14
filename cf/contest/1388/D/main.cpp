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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<i64> a(n);
  vector<int> b(n);  // parent
  REP(i, n) cin >> a[i];
  vector<int> roots;
  vector<vector<int>> child(n);
  REP(i, n) {
    cin >> b[i];
    if (b[i] == -1) {
      roots.push_back(i);
    } else {
      b[i]--;
      child[b[i]].push_back(i);
    }
  }

  vector<vector<int>> seq1(n), seq2(n);
  vector<i64> scores(n);
  auto dfs_score = [&](auto rec, int v) -> i64 {
    i64 res = a[v];
    for (int u : child[v]) {
      i64 s = rec(rec, u);
      if (s >= 0) {
        res += s;
        seq1[v].push_back(u);
      } else {
        seq2[v].push_back(u);
      }
    }
    scores[v] = res;
    return res;
  };

  for (int r : roots) {
    dfs_score(dfs_score, r);
  }

  i64 ans = 0;
  REP(i, n) { ans += scores[i]; }
  cout << ans << endl;

  REP(i, n) {
    DEBUG(i, scores[i], seq1[i].size(), seq2[i].size());
    for (auto x : seq1[i]) DEBUG(x);
    for (auto x : seq2[i]) DEBUG(x);
    cerr << "-=--" << endl;
  }

  auto dfs_out = [&](auto rec, int v, vector<int>& out) -> void {
    for (auto u : seq1[v]) {
      rec(rec, u, out);
    }
    out.push_back(v);
    for (auto u : seq2[v]) {
      rec(rec, u, out);
    }
  };
  vector<int> res;
  for (auto r : roots) {
    dfs_out(dfs_out, r, res);
  }
  for (auto v : res) cout << (v + 1) << ' ';
  cout << endl;
}
