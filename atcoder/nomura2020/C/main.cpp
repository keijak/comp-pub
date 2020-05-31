#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

i64 solve(int N, vector<i64>& A) {
  assert(int(A.size()) == N + 1);
  assert(A.back() > 0);
  if (N == 0) {
    if (A[0] == 1) {
      return 1;
    } else {
      return -1;
    }
  }
  i64 p2 = 1;
  for (int i = 0; i <= N; ++i) {
    if (A[i] > p2 - 1) {
      return -1;
    }
    if (p2 <= 200'000'000LL) {
      p2 <<= 1;
    }
  }

  // Create a spine (imos).
  vector<i64> inners(N + 1);
  inners[0] += 1;
  inners[N] -= 1;

  int d = 0;  // depth where some empty slots are available
  i64 slots = 1;
  vector<i64> inners_acc(N + 2);
  inners_acc[1] = inners[0];
  i64 leaves_cnt = 1;  // leaves already put, below d
  for (int i = 1; i <= N; ++i) {
    i64 a = A[i];  // reduce it.
    if (i == N) a -= 1;
    if (a == 0) continue;
    // cerr << ">> depth " << i << ": put " << a << " leaves" << endl;
    for (;;) {
      // cerr << ">>> ancestor " << d << " slots " << slots << endl;
      if (slots >= a) {
        slots -= a;
        leaves_cnt += a;
        inners[d + 1] += a;
        inners[i] -= a;
        // cerr << ">>> ok, slots -> " << slots << " range(" << (d + 1) << ", "
        //     << (i) << ") updated.\n";
        break;
      }
      a -= slots;
      leaves_cnt += slots;
      inners[d + 1] += slots;
      inners[i] -= slots;
      if (++d == i) return -1;
      leaves_cnt -= A[d];
      inners_acc[d + 1] = inners_acc[d] + inners[d];
      slots = inners_acc[d + 1] * 2 - leaves_cnt;
      // cerr << ">> num inner nodes at depth " << d << ": " << num_inner
      //      << endl;
      // cerr << ">> slots gained: " << slots << " ancestor " << d << endl;
    }
  }

  // vector<i64> acc(N + 2);
  for (int i = d + 1; i <= N; ++i) {
    inners_acc[i + 1] = inners_acc[i] + inners[i];
    // cerr << "> " << (i + 1) << ": " << acc[i + 1] << endl;
  }
  assert(inners_acc[N + 1] == 0);

  i64 ans = 0;
  for (auto x : inners_acc) ans += x;
  for (auto x : A) ans += x;
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> A(N + 1);
  for (auto& x : A) cin >> x;
  cout << solve(N, A) << endl;
}