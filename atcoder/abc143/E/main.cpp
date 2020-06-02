#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

const int INF = 500;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  i64 L;
  cin >> N >> M >> L;
  vector<vector<pair<int, i64>>> adj(N);
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    adj[a].emplace_back(b, c);
    adj[b].emplace_back(a, c);
  }

  vector<vector<pair<int, i64>>> ans(N, vector<pair<int, i64>>(N, {-INF, 0}));
  vector<bool> cached(N);
  auto search = [&](int start, vector<pair<int, i64>>& refills) -> void {
    if (cached[start]) return;
    cached[start] = true;
    priority_queue<tuple<int, i64, int>> pq;
    pq.emplace(0, L, start);
    while (!pq.empty()) {
      int refill = -get<0>(pq.top());
      i64 gas = get<1>(pq.top());
      int city = get<2>(pq.top());
      pq.pop();
      auto state = make_pair(-refill, gas);
      if (state <= refills[city]) continue;
      refills[city] = state;
      for (auto edge : adj[city]) {
        int ncity = edge.first;
        i64 cost = edge.second;
        auto state = make_pair(-refill, gas - cost);
        if (cost <= gas && state > refills[ncity]) {
          pq.emplace(-refill, gas - cost, ncity);
        }
        auto state2 = make_pair(-refill - 1, L - cost);
        if (cost <= L && state2 > refills[ncity]) {
          pq.emplace(-refill - 1, L - cost, ncity);
        }
      }
    }
  };

  int Q;
  cin >> Q;
  REP(i, Q) {
    int s, t;
    cin >> s >> t;
    s--;
    t--;
    search(s, ans[s]);
    int refill = -ans[s][t].first;
    cout << (refill == INF ? -1 : refill) << '\n';
  }
}
