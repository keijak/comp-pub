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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

const i64 INF = 1LL << 50;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<vector<i64>> a(n, vector<i64>(n));
  REP(i, n) REP(j, n) { cin >> a[i][j]; }
  auto solve = [&]() -> i64 {
    MinHeap<tuple<i64, int, int>> heap;
    REP(i, n) REP(j, n) {
      if (a[i][j] != a[j][i]) return -1;
      heap.emplace(a[i][j], i, j);
    }
    vector<vector<i64>> g(n, vector<i64>(n, -1));
    while (heap.size()) {
      auto [cost, i, j] = heap.top();
      heap.pop();
      if (g[i][j] >= 0 && g[i][j] < cost) return -1;
      i64 mincost = INF;
      REP(k, n) {
        if (g[i][k] >= 0 && g[k][j] >= 0) {
          if (g[i][k] + g[k][j] == cost) {
            // ok
          } else if (g[i][k] + g[k][j] < cost) {
            return -1;
          } else {
            g[i][j]
          }
        }
      }
    }

    //
    return 5;
  };
  cout << solve() << endl;
}
