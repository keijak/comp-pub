#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <vector>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

struct Input {
  operator int() { return read_int<int>(); }
  operator long long() { return read_int<long long>(); }

 private:
  template <typename T>
  T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if (ch == '-') {
      sgn = -1;
      ch = getchar_unlocked();
    }
    for (; isdigit(ch); ch = getchar_unlocked()) ret = (ret * 10) + (ch - '0');
    ungetc(ch, stdin);
    return sgn * ret;
  }
} input;

int a[100005], b[100005];

int main() {
  int n = input;
  REP(i, n) a[i] = input;
  REP(i, n) b[i] = input;
  sort(a, a + n);
  sort(b, b + n);
  i64 ans = 0;
  REP(i, n) { ans += std::abs(a[i] - b[i]); }
  printf("%lld\n", ans);
}
