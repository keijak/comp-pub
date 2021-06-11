#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;
using namespace std;

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<int> A(n), B(m);
  REP(i, n) cin >> A[i];
  REP(i, m) cin >> B[i];
  REP(i, std::lcm(n, m)) {
    if (A[i % n] == B[i % m]) return i + 1;
  }
  return -1;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
