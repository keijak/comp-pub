#include <iostream>
#include <vector>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N, M;
  cin >> N >> M;
  if (M >= N) {
    cout << 0 << endl;
    return 0;
  }
  vector<i64> X(M);
  for (auto& x : X) cin >> x;

  auto enough = [&](const i64 steps) -> bool {
    i64 gap = X[0] - 1;
    if (steps < gap) return false;
    i64 offset = max((steps - gap) / 2, steps - 2 * gap);
    for (int i = 1; i < M; ++i) {
      gap = max(0LL, X[i] - X[i - 1] - 1 - offset);
      if (steps < gap) return false;
      offset = max((steps - gap) / 2, steps - 2 * gap);
    }
    return offset >= N - X[M - 1];
  };

  i64 ng = 0, ok = 2 * N;
  while (ok - ng > 1) {
    i64 m = (ok + ng) / 2;
    if (enough(m)) {
      ok = m;
    } else {
      ng = m;
    }
  }
  cout << ok << endl;
}
