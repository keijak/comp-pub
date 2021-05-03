#include <cstdio>

int spf[10'000'001];
int dp[10'000'001];

void sieve_smallest_prime_factors(int n) {
  for (int i = 1; i <= n; ++i) spf[i] = i;
  for (int i = 2; i <= n; i += 2) spf[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (spf[i] != i) continue;
    for (int j = i * i; j <= n; j += i) {
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
    int m = x;
    do {
      m /= p;
    } while (m % p == 0);
    dp[x] = dp[m] + 1;
    if (dp[x] >= k) {
      ++ans;
    }
  }
  printf("%d\n", ans);
}
