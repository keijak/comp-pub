#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

using namespace std;
using i64 = long long;
using u64 = unsigned long long;

using Int = __int128_t;
const i64 LIMIT = i64(1e18) + 1;

struct AlmostFibonacciKnapsack {
  vector<i64> a, acc, C;
  int n;

  bool f(i64 x, int i) {
    if (i < 0) return false;
    if (a[i] == x) {
      C[i] = true;
      return true;
    }
    if (a[i] < x) {
      C[i] = true;
      if (f(x - a[i], i - 1)) return true;
      C[i] = false;
    }
    if (acc[i] < x) return false;
    return f(x, i - 1);
  }

  vector<int> getIndices(long long x) {
    a.resize(2);
    a[0] = 2;
    a[1] = 3;
    for (int i = 2;; ++i) {
      Int y = a[i - 2] + a[i - 1] - 1;
      if (y > LIMIT) break;
      a.push_back(y);
    }
    n = a.size();
    acc.assign(n + 1, 0);
    REP(i, n) acc[i + 1] = acc[i] + a[i];

    // for (int i = 0; i < n; ++i) {
    //   DEBUG(i, a[i], acc[i + 1]);
    // }

    C.assign(n, 0);
    vector<int> ans;
    if (f(x, n - 1)) {
      for (int i = 0; i < n; ++i) {
        if (C[i]) {
          ans.push_back(i + 1);
        }
      }
    } else {
      ans.push_back(-1);
    }
    return ans;
  }
};

int main() {
  AlmostFibonacciKnapsack solver;
  //   DEBUG(solver.getIndices(148));
  //   DEBUG(solver.getIndices(2));
  //   DEBUG(solver.getIndices(13));
  //   DEBUG(solver.getIndices(86267769395LL));
}
