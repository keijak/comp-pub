#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(long long X) {
  long long n = 100;
  for (int i = 0;; i++) {
    if (n >= X) {
      cout << i << endl;
      return;
    }
    n = 1.01 * n;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long X;
  scanf("%lld", &X);
  solve(X);
}
