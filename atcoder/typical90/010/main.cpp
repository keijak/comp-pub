#pragma GCC target("avx")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <type_traits>
using uint = unsigned;

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

uint acc[200002];

int main() {
  const uint n = input;
  const uint n2 = n * 2;
  for (uint i = 0; i < n2; i += 2) {
    const uint c = uint(input) - 1u;
    const uint p = input;
    const uint v = 1 ^ c;
    acc[i + 2 + c] = acc[i + c] + p;
    acc[i + 2 + v] = acc[i + v];
  }
  const uint q = input;
  for (uint i = 0; i < q; ++i) {
    const uint l = (uint(input) - 1u) << 1u;
    const uint r = uint(input) << 1u;
    const uint s1 = acc[r] - acc[l];
    const uint s2 = acc[r + 1] - acc[l + 1];
    printf("%u %u\n", s1, s2);
  }
}
