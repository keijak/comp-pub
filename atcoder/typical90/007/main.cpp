#include <algorithm>
#include <cctype>
#include <cstdio>
#include <type_traits>
#include <vector>

using namespace std;

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

const int BIG = numeric_limits<int>::max();

int a[300003];

int main() {
  const int n = input;
  for (int i = 0; i < n; ++i) {
    a[i] = input;
  }
  sort(a, a + n);
  const int q = input;
  for (int i = 0; i < q; ++i) {
    const int b = input;
    auto it = lower_bound(a, a + n, b);
    int ans = BIG;
    if (it != a + n) {
      ans = min(ans, abs(b - *it));
    }
    if (it != a) {
      ans = min(ans, abs(b - *--it));
    }
    printf("%d\n", ans);
  }
}
