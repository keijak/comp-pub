#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> A(N), B(N);
  vector<pair<i64, i64>> se, es;

  REP(i, N) {
    i64 s, e;
    cin >> s >> e;
    se.emplace_back(s, e);
    es.emplace_back(e, s);
  }
  sort(es.begin(), es.end());
  sort(se.rbegin(), se.rend());

  i64 ans;
  if (N % 2 == 1) {
    ans = es[N / 2].first - (se[N / 2].first - 1);
  } else {
    i64 e1 = es[N / 2 - 1].first, e2 = es[N / 2].first;
    i64 s1 = se[N / 2].first, s2 = se[N / 2 - 1].first;
    ans = (e1 + e2) - (s1 + s2 - 1);
  }
  cout << ans << endl;
}
