#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(long long N, long long M, std::vector<long long> P,
           std::vector<long long> Y) {
  unordered_map<int64, vector<int>> years;
  REP(i, M) { years[P[i]].push_back(Y[i]); }
  unordered_map<int64, unordered_map<int, int>> yeard;
  for (auto& kv : years) {
    sort(kv.second.begin(), kv.second.end());
    REP(j, kv.second.size()) { yeard[kv.first][kv.second[j]] = j + 1; }
  }
  REP(i, M) {
    int x = yeard[P[i]][Y[i]];
    printf("%06d%06d\n", P[i], x);
  }
}

int main() {
  long long N;
  scanf("%lld", &N);
  long long M;
  scanf("%lld", &M);
  std::vector<long long> P(M);
  std::vector<long long> Y(M);
  for (int i = 0; i < M; i++) {
    scanf("%lld", &P[i]);
    scanf("%lld", &Y[i]);
  }
  solve(N, M, std::move(P), std::move(Y));
}
