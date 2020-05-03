#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

int64 solve(int N, vector<int64> A) {
  int64 res = 0;
  unordered_map<int64, int64> m;
  REP(i, N) {
    auto it = m.find(i - A[i]);
    if (it != m.end()) {
      res += it->second;
    }
    m[A[i] + i] += 1;
  }
  return res;
}

int main() {
  int N;
  cin >> N;
  vector<int64> A(N);
  for (auto&& x : A) {
    cin >> x;
  }
  cout << solve(N, move(A)) << endl;
}
