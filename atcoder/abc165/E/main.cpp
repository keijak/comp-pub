#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(int N, int M) {
  vector<pair<int, int>> matches;
  int t0 = N / 2 - (N % 2 == 0);
  int h0 = 0;
  int h1 = N - 1;
  int t1 = h1 - (t0 - 1);
  for (;;) {
    matches.emplace_back(h0, t0);
    h0++;
    t0--;
    if (matches.size() == M) break;
    matches.emplace_back(h1, t1);
    h1--;
    t1++;
    if (matches.size() == M) break;
  }
  REP(i, M) {
    cout << (matches[i].first + 1) << ' ' << (matches[i].second + 1) << '\n';
  }
}

int main() {
  int N, M;
  cin >> N >> M;
  solve(N, M);
}
