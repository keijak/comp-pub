#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N, K;
  cin >> N >> K;
  vector<i64> A(N);
  for (auto& x : A) {
    cin >> x;
  }
  vector<i64> acc(N + 1);
  map<i64, int> pcnt;
  pcnt[0]++;
  i64 ans = 0;
  REP(i, N) {
    if (i - K + 1 >= 0) {
      i64 px = (acc[i - K + 1] - ((i + 1) % K) + K) % K;
      pcnt[px]--;
    }
    i64 rem = (acc[i] + A[i]) % K;
    acc[i + 1] = rem;
    i64 p = (rem - 1 - (i % K) + K) % K;
    i64 c = pcnt[p];
    ans += c;
    pcnt[p]++;
  }
  cout << ans << endl;
}
