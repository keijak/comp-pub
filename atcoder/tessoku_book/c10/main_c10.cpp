#include <cstdio>
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
using Int = long long;

int main() {
  Int W;
  scanf("%lld", &W);
  Mint a = 12 * Mint(7).pow((W - 1) % 1000000006);
  printf("%u\n", a.val());
}
