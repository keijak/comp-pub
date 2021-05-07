#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <type_traits>
#include <unordered_map>
#include <vector>

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
    for (; ('0' <= ch) & (ch <= '9'); ch = getchar_unlocked()) {
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

using namespace std;

unsigned a[100005];

int main() {
  const int n = input;
  const int k = input;
  if (k == n) {
    printf("%d\n", n);
    return 0;
  }
  for (int i = 0; i < n; ++i) {
    a[i] = input;
  }

  unordered_map<unsigned, int> counter;
  counter.reserve(1 << 17);
  counter.max_load_factor(0.25);

  int l = 0;
  int ans = 0;
  for (int r = 0; r < n; ++r) {
    ++counter[a[r]];
    while (l < r and (int) counter.size() > k) {
      auto it = counter.find(a[l]);
      if (it->second == 1) {
        counter.erase(it);
      } else {
        --(it->second);
      }
      ++l;
    }
    if (const int x = r - l + 1; ans < x) ans = x;
  }
  printf("%d\n", ans);
}
