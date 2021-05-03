#include <cstdio>

int spf[10'000'001];
int dp[10'000'001];

void sieve_smallest_prime_factors(int n) {
  for (int i = 1; i <= n; ++i) spf[i] = i;
  for (int i = 2; i <= n; i += 2) spf[i] = 2;
  for (int i = 3;; i += 2) {
    int i2 = i * i;
    if (i2 > n) break;
    if (spf[i] != i) continue;
    for (int j = i2; j <= n; j += i) {
      if (spf[j] == j) spf[j] = i;
    }
  }
}

using namespace std;

int main() {
  int n, k;
  scanf("%d%d", &n, &k);
  if (k == 1) {
    printf("%d\n", n - 1);
    return 0;
  }
  sieve_smallest_prime_factors(n);

  int ans = 0;
  for (int x = 2; x <= n; ++x) {
    const int p = spf[x];
    if (x == p) {
      dp[x] = 1;
      continue;
    }
    int m = x / p;
    int z = dp[m];
    if (z < k and m % p != 0) ++z;
    if (z >= k) ++ans;
    dp[x] = z;
  }
  printf("%d\n", ans);
}
