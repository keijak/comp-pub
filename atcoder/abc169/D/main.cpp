#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;
  i64 n = N;
  vector<tuple<i64, int>> fs;
  for (i64 i = 2; i * i <= N; ++i) {
    if (n % i != 0) continue;
    int cnt = 0;
    while (n % i == 0) {
      n /= i;
      cnt++;
    }
    fs.emplace_back(i, cnt);
    if (n == 1) break;
  }
  if (n > 1) {
    fs.emplace_back(n, 1);
  }
  i64 ans = 0;
  for (auto [p, k] : fs) {
    for (int i = 1;; ++i) {
      if (i > k) break;
      ++ans;
      k -= i;
    }
  }
  cout << ans << endl;
}