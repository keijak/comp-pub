#include <algorithm>
#include <deque>
#include <iostream>
#include <optional>
#include <vector>

#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using i64 = long long;
using u64 = unsigned long long;

int N, M;
i64 INF = 1LL << 60;

optional<u64> bfs(const vector<i64>& xv, const vector<i64>& yv,
                  const vector<deque<bool>>& adj) {
  u64 area = 0;
  int num_nodes = xv.size() * yv.size();
  int width = yv.size();
  vector<tuple<int, int>> steps{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  deque<tuple<int, int>> que;
  int c0 = lower_bound(xv.begin(), xv.end(), 0) - xv.begin();
  int r0 = lower_bound(yv.begin(), yv.end(), 0) - yv.begin();
  que.emplace_back(c0, r0);
  vector<bool> visited(num_nodes, false);
  visited[c0 * width + r0] = true;

  while (que.size()) {
    const auto& [c, r] = que.front();
    int cur_node = c * width + r;
    que.pop_front();
    if (c < 0 || c + 1 >= int(xv.size()) || r < 0 || r + 1 >= int(yv.size())) {
      return nullopt;
    }
    if (xv[c + 1] == INF || xv[c] == -INF || yv[r + 1] == INF ||
        yv[r] == -INF) {
      return nullopt;
    }
    area += (xv[c + 1] - xv[c]) * (yv[r + 1] - yv[r]);
    for (const auto& [dc, dr] : steps) {
      int nxt = (c + dc) * width + (r + dr);
      if (nxt < 0 || nxt >= num_nodes) {
        continue;
      }
      if (adj[cur_node][nxt] && !visited[nxt]) {
        visited[nxt] = true;
        // cerr << ">> pos [" << (r + dr) << ", " << (c + dc) << "] = ("
        //      << yv[r + dr] << ", " << xv[c + dc] << ")  <-  (" << yv[r] << ",
        //      "
        //      << xv[c] << ")" << endl;
        que.emplace_back(c + dc, r + dr);
      }
    }
  }

  return area;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M;
  vector<tuple<i64, i64, i64>> verticals(N);
  vector<tuple<i64, i64, i64>> horizontals(M);
  vector<i64> xv, yv;
  xv.reserve(N + 2 * M + 10);
  yv.reserve(2 * N + M + 10);
  xv.push_back(0);
  xv.push_back(INF);
  xv.push_back(-INF);
  yv.push_back(0);
  yv.push_back(INF);
  yv.push_back(-INF);
  REP(i, N) {  // vertical lines
    i64 a, b, c;
    cin >> a >> b >> c;
    verticals[i] = make_tuple(a, b, c);
    yv.push_back(a);
    yv.push_back(b);
    xv.push_back(c);
  }
  REP(i, M) {  // horizontal lines
    i64 d, e, f;
    cin >> d >> e >> f;
    horizontals[i] = make_tuple(d, e, f);
    yv.push_back(d);
    xv.push_back(e);
    xv.push_back(f);
  }
  sort(xv.begin(), xv.end());
  xv.erase(unique(xv.begin(), xv.end()), xv.end());
  sort(yv.begin(), yv.end());
  yv.erase(unique(yv.begin(), yv.end()), yv.end());
  int num_nodes = xv.size() * yv.size();
  int width = yv.size();
  vector<deque<bool>> adj(num_nodes, deque<bool>(num_nodes, true));
  REP(i, N) {
    auto [a, b, c] = verticals[i];
    int r1 = lower_bound(yv.begin(), yv.end(), a) - yv.begin();
    int r2 = lower_bound(yv.begin(), yv.end(), b) - yv.begin();
    int ci = lower_bound(xv.begin(), xv.end(), c) - xv.begin();
    for (int rd = r1; rd < r2; ++rd) {
      adj[(ci - 1) * width + rd][ci * width + rd] = false;
      adj[ci * width + rd][(ci - 1) * width + rd] = false;
    }
  }
  REP(i, M) {
    auto [d, e, f] = horizontals[i];
    int ri = lower_bound(yv.begin(), yv.end(), d) - yv.begin();
    int c1 = lower_bound(xv.begin(), xv.end(), e) - xv.begin();
    int c2 = lower_bound(xv.begin(), xv.end(), f) - xv.begin();
    for (int cd = c1; cd < c2; ++cd) {
      adj[cd * width + (ri - 1)][cd * width + ri] = false;
      adj[cd * width + ri][cd * width + (ri - 1)] = false;
    }
  }
  auto result = bfs(xv, yv, adj);
  if (!result) {
    cout << "INF" << endl;
  } else {
    cout << *result << endl;
  }
}
