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

  auto factorize = [&](i64 n) -> i64 {
    i64 num_factors = 1;
    for (i64 k = 2; k * k <= n; ++k) {
      if (n % k == 0) {
        int cnt = 0;
        do {
          n /= k;
          ++cnt;
        } while (n % k == 0);
        num_factors *= (cnt + 1);
      }
    }
    if (n > 1) {
      num_factors *= 2;
    }
    return num_factors;
  };

  auto reach = [&](i64 k) -> bool {
    i64 n = N;
    while (n % k == 0) {
      n /= k;
    }
    return n % k == 1;
  };

  i64 ans = factorize(N - 1) - 1;
  i64 n = N;
  for (i64 k = 2; k * k <= n; ++k) {
    if (n % k == 0) {
      if (reach(k)) {
        // cerr << ">> " << k << " reaches 1" << endl;
        ++ans;
      }
    }
  }
  cout << (ans + 1) << endl;
}