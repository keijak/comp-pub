#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

const i64 INF = 10000000000LL;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;

  auto solve_even = [&]() -> i64 {
    i64 a1 = 0, a2 = 0;
    for (int i = 0; i < N; i += 2) {
      a1 += A[i];
      a2 += A[i + 1];
    }
    return max(a1, a2);
  };

  auto solve_odd = [&]() -> i64 {
    vector<i64> a1l(N + 2), a1r(N + 2), a2l(N + 2), a2r(N + 2);
    for (int i = 2; i < N + 2; i += 2) {
      // a1l[i] := even lane, A[i-2] and left.
      a1l[i] = a1l[i - 2] + A[i - 2];
    }
    for (int i = 3; i < N + 2; i += 2) {
      // a2l[i] := odd lane, A[i-2] and left.
      a2l[i] = a2l[i - 2] + A[i - 2];
    }
    for (int i = N - 1; i >= 0; i -= 2) {
      // a1r[i] := even lane, A[i] and right.
      a1r[i] = a1r[i + 2] + A[i];
    }
    for (int i = N - 2; i >= 0; i -= 2) {
      // a2r[i] := odd lane, A[i] and right.
      a2r[i] = a2r[i + 2] + A[i];
    }

    i64 ans = a2r[1];
    auto chmax = [&](i64 v, string_view msg, int i) -> void {
      if (ans < v) {
        cerr << ">>> " << msg << ": at " << i << endl;
        ans = v;
      }
    };
    for (int i = 0; i < N; i += 2) {
      // even lane, exclude A[i].
      chmax(a1l[i] + a1r[i + 2], "even left + even right", i);
      chmax(a1l[i] + a2r[i + 1], "even left + odd right", i);
      chmax(a2l[i + 1] + a1r[i + 2], "odd left + even right", i);
    }
    return ans;
  };

  cout << (N % 2 == 0 ? solve_even() : solve_odd()) << endl;
}
