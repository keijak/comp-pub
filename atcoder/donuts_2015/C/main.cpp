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
  vector<int> H(N);
  for (auto& x : H) cin >> x;
  set<int> dec;
  REP(i, N) {
    cout << dec.size() << '\n';
    auto it = dec.lower_bound(H[i]);
    dec.erase(dec.begin(), it);
    dec.insert(H[i]);
  }
}