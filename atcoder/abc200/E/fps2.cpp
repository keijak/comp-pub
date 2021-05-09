#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

using namespace std;

int main() {
  i64 n, k;
  cin >> n >> k;
  const i64 n2 = n * 2, n3 = n * 3;

  vector<i64> f2(n2 + 5, 0LL);
  f2[2] = 1;
  f2[n + 2] = -2;
  REP(i, 2) REP(j, n2) f2[j + 1] += f2[j];

  vector<i64> f3(n3 + 5, 0LL);
  f3[3] = 1;
  f3[n + 3] = -3;
  f3[n2 + 3] = 3;
  REP(i, 3) REP(j, n3) f3[j + 1] += f3[j];

  i64 rem = k;
  i64 s, x;
  for (s = 3; rem > f3[s]; ++s) {
    rem -= f3[s];
  }
  for (x = max(s - n * 2, 1LL); rem > f2[s - x]; ++x) {
    rem -= f2[s - x];
  }
  const i64 y = max(s - x - n, 1LL) + rem - 1;
  const i64 z = s - x - y;
  cout << x << " " << y << " " << z << endl;
}
