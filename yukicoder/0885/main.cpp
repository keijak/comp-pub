#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <functional>
#include <type_traits>
#include <vector>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

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

int main() {
  const int n = input;
  vector<int> a(n);
  REP(i, n) a[i] = input;
  const int Q = input;
  vector<int> X(Q);
  REP(i, Q) X[i] = input;
  REP(i, 1, Q) { X[i] = min(X[i], X[i - 1]); }
  vector<i64> ans(Q + 1, 0);
  REP(i, n) {
    int p = a[i];
    int j = 0;
    while (true) {
      int q = lower_bound(X.begin() + j, X.end(), p, greater{}) - X.begin();
      ans[j] += p;
      ans[q] -= p;
      if (q == Q) break;
      p %= X[j];
      j = q;
    }
  }
  REP(i, 1, Q) { ans[i] += ans[i - 1]; }
  REP(i, Q) { printf("%lld\n", ans[i]); }
}
