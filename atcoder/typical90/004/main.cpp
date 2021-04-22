#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <type_traits>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

using namespace std;
using u32 = unsigned;

struct Input {
  inline operator int() { return read_int<int>(); }
  inline operator long long() { return read_int<long long>(); }
  inline operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  static T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; isdigit(ch); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;

u32 a[2002 * 2002], row[2002], col[2002];

int main() {
  u32 H = input;
  u32 W = input;
  REP(i, H) REP(j, W) {
    u32 x = input;
    a[i * W + j] = x;
    row[i] += x;
    col[j] += x;
  }
  REP(i, H) {
    REP(j, W) {
      u32 b = row[i] + col[j] - a[i * W + j];
      printf("%u ", b);
    }
    putchar('\n');
  }
}
