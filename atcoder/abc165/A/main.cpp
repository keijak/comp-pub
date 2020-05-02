#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(long long K, long long A, long long B) {
  for (int i = A; i <= B; ++i) {
    if (i % K == 0) {
      cout << "OK" << endl;
      return;
    }
  }
  cout << "NG" << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long K;
  scanf("%lld", &K);
  long long A;
  scanf("%lld", &A);
  long long B;
  scanf("%lld", &B);
  solve(K, A, B);
}
