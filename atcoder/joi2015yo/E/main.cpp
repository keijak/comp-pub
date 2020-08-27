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

#define chmax(x, y) x = max(x, y)
#define chmin(x, y) x = min(x, y)

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  //   ios::sync_with_stdio(false);
  //   cin.tie(nullptr);

  int H, W;
  cin >> H >> W;
  V<string> grid(H);
  REP(i, H) {
    cin >> grid[i];
    assert((int)grid[i].size() == W);
  }

  const int INF = 1e9;
  V<V<int>> collapse = make_vec(H, W, INF);
  MinHeap<tuple<int, int, int>> heap;
  REP(i, H) REP(j, W) {
    if (grid[i][j] == '.') {
      collapse[i][j] = 0;
      heap.emplace(0, i, j);
    }
  }

  array<int, 8> dx = {-1, 0, 1, -1, 1, -1, 0, 1};
  array<int, 8> dy = {-1, -1, -1, 0, 0, 1, 1, 1};

  while (heap.size()) {
    auto [col, r, c] = heap.top();
    heap.pop();
    assert(col < INF);
    REP(i, 8) {
      int nr = r + dy[i];
      int nc = c + dx[i];
      if (nr < 0 || nr >= H || nc < 0 || nc >= W) continue;
      // if (collapse[nr][nc] < INF) continue;
      if (grid[nr][nc] == '.') continue;
      int defence = grid[nr][nc] - '0';
      if (defence == 9) continue;
      V<int> attacks;
      REP(j, 8) {
        int pr = nr + dy[j];
        int pc = nc + dx[j];
        if (pr < 0 || pr >= H || pc < 0 || pc >= W) continue;
        if (collapse[pr][pc] < INF) {
          attacks.push_back(collapse[pr][pc]);
        }
      }
      if (int(attacks.size()) >= defence) {
        sort(attacks.begin(), attacks.end());
        int c = attacks[defence - 1] + 1;
        if (c < collapse[nr][nc]) {
          collapse[nr][nc] = c;
          heap.emplace(collapse[nr][nc], nr, nc);
        }
      }
    }
  }

  //   REP(i, H) {
  //     REP(j, W) {
  //       if (collapse[i][j] == INF) {
  //         cerr << " # ";
  //       } else if (collapse[i][j] == 0) {
  //         cerr << " . ";
  //       } else {
  //         fprintf(stderr, "%2d ", collapse[i][j]);
  //       }
  //     }
  //     cerr << endl;
  //   }

  int max_collapse = 0;
  REP(i, H) REP(j, W) {
    int c = collapse[i][j];
    if (0 < c && c < INF) {
      chmax(max_collapse, collapse[i][j]);
    }
  }
  cout << max_collapse << endl;
}
