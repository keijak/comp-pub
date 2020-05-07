#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

const long long MOD = 1000000007;

void solve(long long N, std::vector<std::vector<int>> a) {
  vector<unordered_map<int, int64>> dp(N);
  for (auto i : a[0]) {
    dp[0][1 << i] = 1;
  }
  for (int i = 1; i < N; ++i) {
    for (auto&& stcnt : dp[i - 1]) {
      int state = stcnt.first;
      int64 cnt = stcnt.second;
      for (auto j : a[i]) {
        int sj = 1 << j;
        if (state & sj) continue;
        dp[i][state | sj] += cnt;
        dp[i][state | sj] %= MOD;
      }
    }
  }
  int64 ans = 0;
  for (auto&& stcnt : dp[N - 1]) {
    ans = (ans + stcnt.second) % MOD;
  }
  cout << ans << endl;
}

int main() {
  int64 N;
  cin >> N;
  std::vector<std::vector<int>> a(N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int x;
      cin >> x;
      if (x) {
        a[i].push_back(j);
      }
    }
  }
  sort(a.begin(), a.end(),
       [](auto& v1, auto& v2) -> bool { return v1.size() < v2.size(); });
  solve(N, std::move(a));
}
