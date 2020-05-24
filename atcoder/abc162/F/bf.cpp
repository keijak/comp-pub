#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
const i64 INF = 10000000000LL;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  const int NH = N / 2;

  vector<bool> dbg(27);

  auto dfs = [&](auto rec, int i, bool psel, int nsel, i64 msum) -> i64 {
    if (i == N) {
      if (N == 27 && msum == 295) {
        cerr << "DBG:" << endl;
        REP(i, 27) {
          if (dbg[i])
            cerr << ' ' << A[i];
          else
            cerr << " _";
        }
        cerr << endl;
      }
      return msum;
    }
    i64 ans = -INF;
    if (!psel && nsel < NH) {
      dbg[i] = true;
      i64 a = rec(rec, i + 1, true, nsel + 1, msum + A[i]);
      ans = max(ans, a);
    }
    if (nsel + (N - i - 1) >= NH) {
      dbg[i] = false;
      i64 a = rec(rec, i + 1, false, nsel, msum);
      ans = max(ans, a);
    }
    return ans;
  };
  cout << dfs(dfs, 0, false, 0, 0) << endl;
}