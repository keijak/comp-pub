#include <cstdio>

int main() {
  unsigned x, p;
  scanf("%u%u", &x, &p);
  if (x & 1) ++x;
  printf("%f\n", (x >> 1) * 100.0f / p);
}
