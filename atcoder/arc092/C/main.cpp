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
  vector<tuple<int, int, bool>> P;
  P.reserve(2 * N);
  REP(i, N) {
    int x, y;
    cin >> x >> y;
    P.emplace_back(x, y, true);
  }
  REP(i, N) {
    int x, y;
    cin >> x >> y;
    P.emplace_back(x, y, false);
  }
  sort(P.rbegin(), P.rend());

  set<int> ys;
  int ans = 0;
  for (auto& p : P) {
    int y = get<1>(p);
    bool is_red = get<2>(p);
    if (!is_red) {
      ys.insert(y);
      continue;
    }
    auto it = ys.lower_bound(y);
    if (it == ys.end()) {
      continue;
    }
    ans++;
    ys.erase(it);
  }
  cout << ans << endl;
}