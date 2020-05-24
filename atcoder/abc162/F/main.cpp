#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

const i64 INF = 1LL << 60;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  const int skip = (N % 2 == 0) ? 1 : 2;
  // pfilled[i][k] = maxsum at i, where i is selected and there's k room.
  vector<vector<i64>> pfilled(N, vector<i64>(3, -INF));
  // pempty[i][k] = maxsum at i, where i is not selected and there's k room.
  vector<vector<i64>> pempty(N, vector<i64>(3, -INF));

  pempty[0][skip - 1] = 0;
  pfilled[0][skip] = A[0];
  for (int i = 1; i < N; ++i) {
    REP(j, 3) {
      pfilled[i][j] = pempty[i - 1][j];
      if (pfilled[i][j] != -INF) {
        pfilled[i][j] += A[i];
      }
      pempty[i][j] = pfilled[i - 1][j];
      if (j < 2) {
        pempty[i][j] = max(pempty[i][j], pempty[i - 1][j + 1]);
      }
    }
  }

  i64 ans = -INF;
  REP(j, 2) {
    ans = max(ans, pfilled[N - 1][j]);
    ans = max(ans, pempty[N - 1][j]);
  }
  assert(ans != -INF);
  cout << ans << endl;
}