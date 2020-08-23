#include <bits/stdc++.h>

#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ",", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ",", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

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

struct Edge {
  i64 cost;
  int r, c;
  bool operator>(const Edge &other) const { return cost > other.cost; }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int H, W;
  cin >> H >> W;
  int sr, sc;
  cin >> sr >> sc;
  sr--;
  sc--;
  int dr, dc;
  cin >> dr >> dc;
  dr--;
  dc--;
  V<string> grid(H);
  REP(i, H) { cin >> grid[i]; }

  const int INF = 1e8;

  deque<Edge> q;
  V<V<int>> cost(H, V<int>(W, INF));
  q.push_back({0, sr, sc});
  while (!q.empty()) {
    Edge e = move(q.front());
    // auto [d, cr, cc] = hp.top();
    assert(grid[e.r][e.c] == '.');
    q.pop_front();
    if (e.cost > cost[e.r][e.c]) continue;
    cost[e.r][e.c] = e.cost;
    if (e.r == dr && e.c == dc) break;

    for (int dy = -2; dy <= 2; ++dy) {
      for (int dx = -2; dx <= 2; ++dx) {
        if (dx == 0 && dy == 0) continue;
        int r2 = e.r + dy;
        int c2 = e.c + dx;
        if (0 > r2 || r2 >= H || 0 > c2 || c2 >= W) continue;
        if (grid[r2][c2] == '#') continue;
        int co = 1;
        if (dx * dy == 0 && max(abs(dx), abs(dy)) == 1) {
          co = 0;
        }
        if (e.cost + co < cost[r2][c2]) {
          cost[r2][c2] = e.cost + co;
          if (co == 0) {
            q.push_front({e.cost + co, r2, c2});
          } else {
            q.push_back({e.cost + co, r2, c2});
          }
        };
      }
    }
  }

  i64 ans = cost[dr][dc];
  cout << (ans == INF ? -1LL : ans) << endl;
}
