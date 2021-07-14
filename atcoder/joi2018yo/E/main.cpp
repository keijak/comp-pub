#include <bits/stdc++.h>
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
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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

#define chmin(x, y) x = min(x, y)

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int H, W;
  cin >> H >> W;
  auto A = make_vec(H, W, 0LL);
  REP(i, H) REP(j, W) { cin >> A[i][j]; }
  assert(A[0][0] == 0);

  const i64 INF = 1e18;
  auto mincost = make_vec(H, W, H * W, INF);
  mincost[0][0][0] = 0;
  //   auto mindist = make_vec(H, W, INF);
  //   mindist[0][0] = 0;

  array<int, 4> dx = {0, 0, 1, -1};
  array<int, 4> dy = {1, -1, 0, 0};

  MinHeap<tuple<i64, i64, int, int>> heap;
  heap.emplace(0LL, 0LL, 0, 0);
  i64 ans = INF;
  while (heap.size()) {
    auto [cost, dist, r, c] = heap.top();
    heap.pop();
    if (cost > mincost[r][c][dist]) continue;
    if (r == H - 1 && c == W - 1) {
      chmin(ans, cost - dist);
    }
    REP(i, 4) {
      int nr = r + dy[i];
      int nc = c + dx[i];
      if (nr < 0 || nc < 0 || nr >= H || nc >= W) continue;
      i64 new_cost = cost + A[nr][nc] + 2 * A[nr][nc] * dist + 1;
      i64 new_dist = dist + 1;
      if (new_dist < H * W && new_cost < mincost[nr][nc][new_dist]) {
        mincost[nr][nc][new_dist] = new_cost;
        heap.emplace(new_cost, new_dist, nr, nc);
      }
    }
  }

  cout << ans << endl;
}
