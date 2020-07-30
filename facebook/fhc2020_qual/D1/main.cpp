#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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

const i64 INF = 1LL << 50;

i64 solve() {
  i64 N, M;
  cin >> N >> M;
  vector<i64> C(N);
  REP(i, N) {
    cin >> C[i];
    if (C[i] == 0) C[i] = INF;
  }
  if (M >= N - 1) return 0;

  vector<i64> reachcost(N, INF);
  vector<i64> fillcost(N, INF);
  deque<int> minq;

  auto push_right = [&](int i) -> void {
    while (!minq.empty() && fillcost[minq.back()] >= fillcost[i]) {
      minq.pop_back();
    }
    minq.push_back(i);
  };
  auto pop_left = [&](int i) -> void {
    while (!minq.empty() && minq.front() < i) {
      minq.pop_front();
    }
  };

  REP(i, M + 1) {
    reachcost[i] = 0;
    fillcost[i] = C[i];
    push_right(i);
  }
  for (int i = M + 1; i < N; ++i) {
    pop_left(i - M);
    int j = minq.front();
    DEBUG(i, j, fillcost[j]);
    reachcost[i] = fillcost[j];
    if (reachcost[i] == INF) return -1;
    if (C[i] == INF) {
      fillcost[i] = INF;
    } else {
      fillcost[i] = reachcost[i] + C[i];
    }
    DEBUG(fillcost[i], i - M);
    push_right(i);
  }

  i64 ans = reachcost[N - 1];
  assert(ans < INF);
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    cerr << "test" << i << endl;
    DEBUG(i);
    cout << "Case #" << (i + 1) << ": " << solve() << "\n";
  }
}
