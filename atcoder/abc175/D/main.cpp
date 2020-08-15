#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  i64 K;
  cin >> N >> K;
  V<int> P(N);
  REP(i, N) {
    cin >> P[i];
    P[i]--;
  }
  V<i64> C(N);
  REP(i, N) cin >> C[i];

  auto calc = [&](int i) -> i64 {
    V<bool> visited(N);
    int cur = i;
    i64 res = -INF;
    i64 score = 0;
    int cycle = 0;
    while (!visited[cur]) {
      visited[cur] = true;
      int nx = P[cur];
      score += C[nx];
      res = max(res, score);
      ++cycle;
      if (cycle == K) return res;
      cur = nx;
    }
    if (score <= 0) {
      return res;
    }

    i64 m = K / cycle;
    res = max(res, (m - 1) * score + res);
    int cur2 = i;
    i64 res2 = 0;
    i64 score2 = 0;
    REP(j, K % cycle) {
      int nx = P[cur2];
      score2 += C[nx];
      res2 = max(res2, score2);
      cur2 = nx;
    }
    DEBUG(i, score, cycle, m, res2);
    res = max(res, m * score + res2);
    return res;
  };
  i64 ans = -INF;
  REP(i, N) {
    i64 score = calc(i);
    ans = max(ans, score);
  }
  cout << ans << endl;
}
