#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

i64 solve() {
  // Input.
  int N, M, K, S;
  cin >> N >> M >> K >> S;
  assert(S == 0);

  V<i64> P(N), Q(M);
  i64 AP, BP, CP, DP, AQ, BQ, CQ, DQ;

  REP(i, K) { cin >> P[i]; }
  cin >> AP >> BP >> CP >> DP;
  REP(i, K) { cin >> Q[i]; }
  cin >> AQ >> BQ >> CQ >> DQ;

  // Create P and Q.
  for (int i = K; i < N; ++i) {
    P[i] = ((AP % DP) * P[i - 2] % DP + BP % DP * P[i - 1] % DP + CP % DP) % DP;
    P[i]++;
  }
  for (int i = K; i < M; ++i) {
    Q[i] = (AQ % DQ * Q[i - 2] % DQ + BQ % DQ * Q[i - 1] % DQ + CQ % DQ) % DQ;
    Q[i]++;
  }

  sort(P.begin(), P.end());
  sort(Q.begin(), Q.end());

  auto check = [&](i64 timelimit) -> bool {
    int qi = 0;
    for (int pi = 0; pi < N; ++pi) {
      assert(qi < M);
      i64 rpos;
      // DEBUG(timelimit, pi, P[pi], qi, Q[qi]);
      if (Q[qi] < P[pi]) {
        if (Q[qi] < P[pi] - timelimit) return false;
        i64 remtime = timelimit - (P[pi] - Q[qi]);
        rpos = max<i64>(Q[qi] + remtime, P[pi] + remtime / 2);
      } else {
        rpos = P[pi] + timelimit;
      }
      auto it = upper_bound(Q.begin(), Q.end(), rpos);
      if (it == Q.end()) return true;
      qi = max<int>(qi, it - Q.begin());
    }
    return qi >= M;
  };

  i64 ng = -1, ok = 1LL << 55;
  assert(ok > (Q[M - 1] - Q[0]) * 3LL);
  assert(ok > (P[N - 1] - P[0]) * 3LL);
  while (ok - ng > 1) {
    i64 mid = (ng + ok) / 2LL;
    bool r = check(mid);
    // DEBUG(mid, r);
    if (r) {
      ok = mid;
    } else {
      ng = mid;
    }
  }

  return ok;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DEBUG("case", i + 1);
    if (i % 10 == 0) cerr << "case" << (i + 1) << endl;
    cout << "Case #" << (i + 1) << ": " << solve() << '\n';
  }
}
