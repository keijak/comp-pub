#include <bits/stdc++.h>

using i64 = long long;

using namespace std;

// binom(n+1, 1)
i64 binom1(i64 n) { return n + 1; }

// binom(n+1, 2)
i64 binom2(i64 n) { return (n + 2) * (n + 1) / 2; }

int main() {
  i64 n, k;
  cin >> n >> k;
  const i64 n2 = n * 2, n3 = n * 3;

  vector<i64> f2(n2 + 1, 0);
  for (int i = 0; i <= n2; ++i) {
    if (i + 2 <= n2) {
      f2[i + 2] += binom1(i);
    }
    if (i + n + 2 <= n2) {
      f2[i + n + 2] += -2 * binom1(i);
    }
    if (i + n2 + 2 <= n2) {
      f2[i + n2 + 2] += binom1(i);
    }
  }

  vector<i64> f3(n3 + 1, 0);
  for (int i = 0; i <= n3; ++i) {
    if (i + 3 <= n3) {
      f3[i + 3] += binom2(i);
    }
    if (i + n + 3 <= n3) {
      f3[i + n + 3] += -3 * binom2(i);
    }
    if (i + n2 + 3 <= n3) {
      f3[i + n2 + 3] += 3 * binom2(i);
    }
    if (i + n3 + 3 <= n3) {
      f3[i + n3 + 3] += -binom2(i);
    }
  }

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
