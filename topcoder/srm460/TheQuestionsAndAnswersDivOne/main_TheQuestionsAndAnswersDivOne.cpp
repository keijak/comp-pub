#include "gtest/gtest.h"

#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

using namespace std;

inline int popcount(unsigned x) { return __builtin_popcount(x); }

template<class T>
std::vector<std::vector<T>> comb_table(int n) {
  std::vector<std::vector<T>> C(n + 1, std::vector<T>(n + 1, 0));
  for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C;
}

template<typename T>
T power(T b, Int e) {
  if (b == 0 and e == 0) {
    return 1;
  }
  T x = 1;
  while (e > 0) {
    if (e & 1) x *= b;
    b *= b;
    e >>= 1;
  }
  return x;
}

struct TheQuestionsAndAnswersDivOne {
  int find(int q, vector<string> answers) {
    const int M = (int) answers.size();
    int ya = 0;
    for (auto a: answers) if (a == "Yes") ++ya;
    int na = M - ya;
    auto C = comb_table<Int>(10);

    Int ans = 0;
    REP(yesno, 1 << q) {
      int yk = popcount(yesno);
      int nk = q - yk;
      if (yk > ya or nk > na) continue;
      REP(yc, yk + 1) {
        REP(nc, nk + 1) {
          Int co = power<Int>(yk - yc, ya) * power<Int>(nk - nc, na);
          co *= C[yk][yc] * C[nk][nc];
          int sign = ((yc + nc) & 1) ? -1 : 1;
          ans += sign * co;
        }
      }
    }
    return ans;
  }
};

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

TEST(TheQuestionsAndAnswersDivOne, Sample) {
  TheQuestionsAndAnswersDivOne t;
  EXPECT_EQ(2, t.find(2, {"No", "Yes"}));
  EXPECT_EQ(6, t.find(2, {"No", "No", "No"}));
  EXPECT_EQ(12, t.find(3, {"Yes", "No", "No", "Yes"}));
  EXPECT_EQ(18, t.find(3, {"Yes", "Yes", "Yes", "No"}));
}
