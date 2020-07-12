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
  int N;
  cin >> N;
  vector<int> A(N);
  A[0] = -1;
  REP(i, N - 1) {
    cin >> A[i + 1];
    --A[i + 1];
  }

  vector<int> wins(N);
  REP(i, N - 1) { ++wins[A[i + 1]]; }

  vector<vector<int>> losers(N);
  REP(i, N - 1) {
    int loser = i + 1;
    int winner = A[loser];
    losers[winner].push_back(loser);
  }

  auto dfs = [&](auto rec, int v) -> int {
    vector<int> ds;
    ds.reserve(losers[v].size());
    for (auto u : losers[v]) {
      ds.push_back(rec(rec, u));
    }
    int maxdepth = wins[v];
    sort(ds.rbegin(), ds.rend());
    REP(i, ds.size()) {
      int d = ds[i] + i + 1;
      if (maxdepth < d) maxdepth = d;
    }
    return maxdepth;
  };
  int maxdepth = dfs(dfs, 0);
  cout << maxdepth << endl;
}
