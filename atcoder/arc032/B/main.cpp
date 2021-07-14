#include <bits/stdc++.h>

#include <atcoder/dsu>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)

using namespace std;
using atcoder::dsu;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  dsu uf(n);

  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    uf.merge(a, b);
  }
  cout << uf.groups().size() - 1 << '\n';
}
