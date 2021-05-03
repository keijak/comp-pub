#include <cstdio>

unsigned count[10'000'001];

using namespace std;

int main() {
  unsigned n, k;
  scanf("%u%u", &n, &k);
  if (k == 1) {
    printf("%u\n", n - 1);
    return 0;
  }
  for (unsigned i = 4; i <= n; i += 2) {
    ++count[i];
  }
  const unsigned m = (n + 1) / 2;
  for (unsigned i = 3; i <= m; i += 2) {
    if (count[i]) continue;
    for (unsigned j = i << 1; j <= n; j += i) {
      ++count[j];
    }
  }
  unsigned ans = 0;
  for (unsigned i = 2; i <= n; ++i) {
    if (count[i] >= k) ++ans;
  }
  printf("%u\n", ans);
}
