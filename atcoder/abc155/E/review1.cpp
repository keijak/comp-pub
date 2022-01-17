#include <cstdio>
#include <algorithm>

char buf[1'000'010];

int main() {
  int a = 0, b = 1, i = 0;
  char c;
  fgets_unlocked(buf, 1'000'005, stdin);
  while ((c = buf[i++]) != '\n') {
    char d = c - '0';
    int a_ = std::min<int>(a + d, b + 10 - d);
    b = std::min<int>(a + d + 1, b + 9 - d);
    a = a_;
  }
  printf("%d\n", a);
}
