#define NDEBUG
#include <bits/stdc++.h>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <utility>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using Int = long long;
using Uint = unsigned long long;

template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;

template<typename T>
void read_unsigned(T &ret) {
  // ret = 0;  // assumption: ret is already zero-initialized.
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked());
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

constexpr int N = 700010;
int gs[N * 2];
int gl = N, gr = N;
Uint A[N], C[N];

auto solve() -> Int {
  unsigned n = 0, L = 0, K = 0;
  read_unsigned(n);
  read_unsigned(L);
  read_unsigned(K);
  REP(i, n) {
    read_unsigned(A[i]);
    read_unsigned(C[i]);
  }
  A[n] = L;
  C[n] = kBig<Int>;
  Int x = 0, s = K, r = 0;
  Int cost = 0;
  while (true) {
    Int x2 = x + s;
    if (x2 >= L) {
      return cost;
    }
    for (; r <= n and A[r] <= x2; ++r) {
      while (gl != gr and C[gs[gr - 1]] > C[r]) --gr;
      gs[gr++] = r;
    }
    while (x2 < A[r] and gl != gr) {
      int j = gs[gl];
      if (A[j] >= x) {
        s -= A[j] - x;
        x = A[j];
        Int fill = min<Int>(K - s, A[r] - x2);
        cost += C[j] * fill;
        s += fill;
        x2 += fill;
        if (s < K) {
          break;
        }
      }
      ++gl;
    }
    if (x2 < A[r]) return -1;
  }
}

int main() {
  printf("%lld\n", solve());
  exit_();
}
