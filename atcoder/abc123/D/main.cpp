#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int X, Y, Z, K;
  cin >> X >> Y >> Z >> K;
  vector<i64> A(X), B(Y), C(Z);
  for (auto& x : A) cin >> x;
  for (auto& x : B) cin >> x;
  for (auto& x : C) cin >> x;
  sort(A.rbegin(), A.rend());
  sort(B.rbegin(), B.rend());
  sort(C.rbegin(), C.rend());
  priority_queue<tuple<i64, int, int, int>> que;
  set<tuple<int, int, int>> done;
  que.emplace(A[0] + B[0] + C[0], 0, 0, 0);
  auto maybe_push = [&](int ai, int bi, int ci) -> void {
    auto tpl = make_tuple(ai, bi, ci);
    if (ai < X && bi < Y && ci < Z && !done.count(tpl)) {
      done.insert(tpl);
      que.emplace(A[ai] + B[bi] + C[ci], ai, bi, ci);
    }
  };
  int cnt = 0;
  while (que.size()) {
    i64 sm = get<0>(que.top());
    int ai = get<1>(que.top());
    int bi = get<2>(que.top());
    int ci = get<3>(que.top());
    que.pop();
    cout << sm << "\n";
    if (++cnt == K) break;
    maybe_push(ai + 1, bi, ci);
    maybe_push(ai, bi + 1, ci);
    maybe_push(ai, bi, ci + 1);
  }
}
