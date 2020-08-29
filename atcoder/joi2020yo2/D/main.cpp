#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
using u128 = __uint128_t;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
void chmax(T &a, T b) {
  if (a < b) a = std::move(b);
}
template <class T>
void chmin(T &a, T b) {
  if (a > b) a = std::move(b);
}

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

const int inf = 1e6;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // Build dist.
  auto dist = make_vec(10, 10, inf);
  REP(i, 10) { dist[i][i] = 0; }
  V<tuple<int, int>> edges = {{1, 2}, {2, 3}, {4, 5}, {5, 6}, {7, 8},
                              {8, 9}, {0, 1}, {1, 4}, {4, 7}, {2, 5},
                              {5, 8}, {3, 6}, {6, 9}};
  for (auto [x, y] : edges) {
    dist[x][y] = dist[y][x] = 1;
  }
  REP(k, 10) REP(i, 10) REP(j, 10) {
    chmin(dist[i][j], dist[i][k] + dist[k][j]);
  }
  // REP(i, 10) { DEBUG(i, dist[i]); }

  int M, R;
  cin >> M >> R;

  MinHeap<tuple<int, int, int>> q;
  q.emplace(0, 0, 0);
  V<V<int>> mincost(10, V<int>(M, inf));
  mincost[0][0] = 1;
  while (q.size()) {
    auto [cost, cur, rem] = q.top();
    q.pop();
    if (cost > mincost[cur][rem]) {
      continue;
    }
    if (rem == R) {
      cout << cost << endl;
      return 0;
    }

    REP(nex, 10) {
      int dco = dist[cur][nex];
      if (dco == inf) continue;
      int nco = cost + dco + 1;
      int nrem = (rem * 10 + nex) % M;
      if (nco < mincost[nex][nrem]) {
        mincost[nex][nrem] = nco;
        q.emplace(nco, nex, nrem);
      }
    }
  }
}
