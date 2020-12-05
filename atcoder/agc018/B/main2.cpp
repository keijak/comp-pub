#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

i64 solve() {
  int n, m;
  cin >> n >> m;
  auto A = vector(n, vector(m, 0));
  REP(i, n) REP(j, m) {
    int sp;
    cin >> sp;
    A[i][j] = --sp;
  }
  REP(i, n) reverse(ALL(A[i]));

  int ans = n;
  bitset<300> used;
  vector<int> count(m);
  while (true) {
    fill(ALL(count), 0);
    int max_count = 0;
    REP(i, n) {
      int sp = A[i].back();
      max_count = max(max_count, ++count[sp]);
    }
    ans = min(ans, max_count);
    REP(i, m) {
      if (count[i] >= max_count) used.set(i, true);
    }

    REP(i, n) {
      while (not A[i].empty()) {
        int sp = A[i].back();
        if (not used[sp]) break;
        A[i].pop_back();
      }
      if (A[i].empty()) goto func_end;
    }
  }

func_end:
  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
