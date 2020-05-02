#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(long long A, long long B, long long N) {
  auto x = min<int64>(B - 1, N);
  int64 y = (A * x) / B - A * (x / B);
  cout << y << endl;
}

int main() {
  long long A;
  scanf("%lld", &A);
  long long B;
  scanf("%lld", &B);
  long long N;
  scanf("%lld", &N);
  solve(A, B, N);
}
