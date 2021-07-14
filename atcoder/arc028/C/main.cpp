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
  vector<vector<int>> adj(N);
  REP(i, N - 1) {
    int p;
    cin >> p;
    adj[i + 1].push_back(p);
    adj[p].push_back(i + 1);
  }

  vector<int> sizes(N), maxc(N), bal(N);
  auto calcsize = [&](auto rec, int cur, int par) -> int {
    int sz = 1;
    int msz = 0;
    for (auto nx : adj[cur]) {
      if (nx == par) continue;
      int csz = rec(rec, nx, cur);
      sz += csz;
      msz = max(msz, csz);
    }
    sizes[cur] = sz;
    maxc[cur] = msz;
    bal[cur] = max(msz, N - sz);
    return sz;
  };
  calcsize(calcsize, 0, -1);

  REP(i, N) { cout << bal[i] << endl; }
}
