#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;

using namespace std;

const int BIG = 1e9;

struct OrderOfOperations {
  int minTime(vector<string> s) {
    int n = s.size();
    int m = s[0].size();
    set<int> a;
    unsigned target = 0;
    for (int i = 0; i < n; ++i) {
      int val = 0;
      int cnt = 0;
      for (int j = 0; j < m; ++j) {
        if (s[i][j] == '1') {
          val |= 1 << j;
          ++cnt;
        }
      }
      a.insert(val);
      target |= val;
    }
    vector<unsigned> dp(1 << m, BIG);
    dp[0] = 0;
    for (int bits = 0; bits != target; ++bits) {
      if (dp[bits] == BIG) continue;
      for (auto x : a) {
        unsigned mask = x & ~bits;
        if (mask == 0) continue;
        int k = __builtin_popcount(mask);
        unsigned nb = bits | mask;
        dp[nb] = min(dp[nb], dp[bits] + k * k);
      }
    }
    return dp[target];
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  OrderOfOperations solver;
  assert(solver.minTime({"111", "001", "010"}) == 3);
  assert(solver.minTime({"11101", "00111", "10101", "00000", "11000"}) == 9);
  assert(solver.minTime({"11111111111111111111"}) == 400);
  assert(solver.minTime({"1000", "1100", "1110"}) == 3);
  assert(solver.minTime({"111", "111", "110", "100"}) == 3);
  auto a = solver.minTime({"1100", "0011", "0111"});
  assert(a == 6);
  a = solver.minTime({"0011", "1100", "0111"});
  assert(a == 6);

  cout << "ok" << endl;
}
