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

template <class T>
using MinHeap = std::priority_queue<T, V<T>, greater<T>>;
//__gnu_pbds::priority_queue<T, greater<T>, __gnu_pbds::rc_binomial_heap_tag>;

struct Edge {
  i64 cost;
  int r, c;
  bool operator>(const Edge &other) const {
    if (cost != other.cost) return cost > other.cost;
    if (r != other.r) return r > other.r;
    return c > other.c;
  }
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

  MinHeap<Edge> hp;
  V<V<int>> cost(H, V<int>(W, INF));
  hp.push({0, sr, sc});
  while (!hp.empty()) {
    Edge e = move(hp.top());
    // auto [d, cr, cc] = hp.top();
    assert(grid[e.r][e.c] == '.');
    hp.pop();
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
          hp.push({e.cost + co, r2, c2});
        };
      }
    }
  }

  i64 ans = cost[dr][dc];
  cout << (ans == INF ? -1LL : ans) << endl;
}
