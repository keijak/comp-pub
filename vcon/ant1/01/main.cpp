#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

const i64 INF = 1LL << 50;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, M;
  cin >> N >> M;
  vector<i64> P(N);
  for (auto& x : P) cin >> x;
  set<i64> s2;
  REP(i, N) REP(j, N) {
    if (P[i] + P[j] <= M) {
      s2.insert(P[i] + P[j]);
    }
  }
  i64 ans = INF;
  REP(i, N) {
    if (P[i] > M) continue;
    ans = min(ans, M - P[i]);
    auto it = s2.upper_bound(M - P[i]);
    if (it == s2.begin()) continue;
    --it;
    i64 x = P[i] + *it;
    assert(M - x >= 0);
    ans = min(ans, M - x);
  }
  for (i64 y : s2) {
    if (y > M) continue;
    ans = min(ans, M - y);
    auto it = s2.upper_bound(M - y);
    if (it == s2.begin()) continue;
    --it;
    i64 x = y + *it;
    assert(M - x >= 0);
    ans = min(ans, M - x);
  }
  if (ans == INF) {
    cout << 0 << endl;
  } else {
    cout << (M - ans) << endl;
  }
}
