#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
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
template <typename T>
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct State {
  i64 cost;
  int y;
  int x;
  int slip;
  int big_leap;
};
bool operator>(const State &x, const State &y) {
  if (x.cost != y.cost) return x.cost > y.cost;
  return x.big_leap < y.big_leap;
}

const int XMAX = 1000;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;  // <= 150
  cin >> N >> M;
  V<map<int, int>> slip(N);
  const i64 INF = 1e18;
  auto mincost = make_vec(M + 1, N, map<int, i64>());
  REP(i, N) {
    int k;  // <= 10
    cin >> k;
    REP(j, k) {
      int x, d;  // <= 1000
      cin >> x >> d;
      slip[i][x] = d;
      REP(m, M + 1) mincost[m][i][x] = INF;
    }
  }
  MinHeap<State> heap;
  for (auto [x, d] : slip[0]) {
    mincost[M][0][x] = 0;
    heap.push({0, 0, x, d, M});
  }
  if (M > 0) {
    for (auto [x, d] : slip[1]) {
      mincost[M - 1][1][x] = 0;
      heap.push({0, 1, x, d, M - 1});
    }
  }

  while (heap.size()) {
    State cur = move(heap.top());
    heap.pop();
    if (cur.cost > mincost[cur.big_leap][cur.y][cur.x]) continue;
    if (cur.y == N - 1 or (cur.y == N - 2 and cur.big_leap > 0)) {
      cout << cur.cost << endl;
      return 0;
    }
    int ny = cur.y + 1;
    for (auto [x, d] : slip[ny]) {
      i64 new_cost = cur.cost + i64(cur.slip + d) * abs(x - cur.x);
      int m = cur.big_leap;
      if (new_cost < mincost[m][ny][x]) {
        mincost[m][ny][x] = new_cost;
        heap.push({new_cost, ny, x, d, m});
      }
    }

    ny = cur.y + 2;
    if (cur.big_leap <= 0 or ny >= N) continue;
    for (auto [x, d] : slip[ny]) {
      i64 new_cost = cur.cost + i64(cur.slip + d) * abs(x - cur.x);
      int m = cur.big_leap - 1;
      if (new_cost < mincost[m][ny][x]) {
        mincost[m][ny][x] = new_cost;
        heap.push({new_cost, ny, x, d, m});
      }
    }
  }

  assert(false);  // never reach here
}
