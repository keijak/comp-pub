#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <deque>
#include <iostream>
#include <optional>
#include <stack>
#include <type_traits>
#include <vector>

template <typename T>
inline void read_unsigned(T& ret) {
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked())
    ;
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
}

int N, K;
char s[100005];
int DQ[100005];
int lp, rp;

int left, right;

inline bool qempty() { return lp >= rp; }

void slide(int l, int r) {
  r = std::min(r, N);

  while (not qempty() && DQ[lp] < l) ++lp;
  left = l;

  for (int i = right; i < r; ++i) {
    while (not qempty() && s[DQ[rp - 1]] > s[i]) --rp;
    DQ[rp++] = i;
  }
  right = r;
}

int main() {
  read_unsigned(N);
  read_unsigned(K);
  fgets(s, N + 1, stdin);
  // s[N] = '\0';
  // std::cerr << "[" << s << "]" << std::endl;

  int tail = N - K + 1;
  slide(0, tail);
  for (int i = 0; i < K; ++i) {
    int j = DQ[lp];
    putchar_unlocked(s[j]);
    slide(j + 1, ++tail);
  }
  putchar_unlocked('\n');
}
