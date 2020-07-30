#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

bool solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  assert(int(s.size()) == n);
  int acnt = 0, bcnt = 0;
  REP(i, n) {
    if (s[i] == 'A')
      ++acnt;
    else
      ++bcnt;
  }
  return abs(acnt - bcnt) == 1;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  REP(i, T) { cout << "Case #" << (i + 1) << (solve() ? ": Y\n" : ": N\n"); }
}
