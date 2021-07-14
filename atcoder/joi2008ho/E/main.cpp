#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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

template <typename T>
struct Compress {
  vector<T> vec;

  explicit Compress(vector<T> v) : vec(v) {
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
  }
  int index(T x) {
    return (int)(lower_bound(vec.begin(), vec.end(), x) - vec.begin());
  }
  T &operator[](int i) { return vec[i]; }
  int size() { return vec.size(); }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int W, H, N;
  cin >> W >> H >> N;
  V<tuple<int, int, int, int>> ps(N);
  V<int> xs = {-1, 0, W - 1, W}, ys = {-1, 0, H - 1, H};
  xs.reserve(2 * N + 10);
  ys.reserve(2 * N + 10);
  REP(i, N) {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    ps[i] = {x1, y1, x2, y2};
    for (int x : {x1, x2}) {
      xs.push_back(x);
    }
    for (int y : {y1, y2}) {
      ys.push_back(y);
    }
  }
  // Coordinate compression
  Compress xc(move(xs)), yc(move(ys));

  // 2-d imos.
  auto grid = make_vec(xc.size(), yc.size(), 0);
  for (const auto &[x1, y1, x2, y2] : ps) {
    int i1 = xc.index(x1), j1 = yc.index(y1);
    int i2 = xc.index(x2), j2 = yc.index(y2);
    grid[i1][j1] -= 1;
    grid[i2][j1] += 1;
    grid[i1][j2] += 1;
    grid[i2][j2] -= 1;
  }
  REP(r, yc.size()) {
    for (int c = 1; c < xc.size(); ++c) {
      grid[c][r] += grid[c - 1][r];
    }
  }
  REP(c, xc.size()) {
    for (int r = 1; r < yc.size(); ++r) {
      grid[c][r] += grid[c][r - 1];
    }
  }
  {  // outlines
    int wi = xc.index(W);
    int hi = yc.index(H);
    REP(i, yc.size()) { grid[0][i] = grid[wi][i] = -1; }
    REP(i, xc.size()) { grid[i][0] = grid[i][hi] = -1; }
  }

  array<int, 4> dx = {0, 0, 1, -1};
  array<int, 4> dy = {1, -1, 0, 0};

  auto bfs = [&](int i0, int j0, int color) -> void {
    grid[i0][j0] = color;
    deque<tuple<int, int>> q;
    q.emplace_back(i0, j0);
    while (q.size()) {
      auto [i, j] = q.front();
      q.pop_front();
      REP(k, 4) {
        int i2 = i + dx[k];
        int j2 = j + dy[k];
        if (i2 < 0 or i2 >= xc.size() or j2 < 0 or j2 >= yc.size()) continue;
        if (grid[i2][j2] != 0) continue;
        grid[i2][j2] = color;
        q.emplace_back(i2, j2);
      }
    }
  };

  int counter = 0;
  REP(i, xc.size()) {
    REP(j, yc.size()) {
      if (grid[i][j] == 0) {
        counter++;
        bfs(i, j, counter);
      }
    }
  }
  cout << counter << endl;

  //   REP(j, yc.size()) {
  //     REP(i, xc.size()) {
  //       int c = grid[i][j];
  //       if (c < 0) {
  //         cerr << "..";
  //       } else {
  //         cerr << ' ' << c;
  //       }
  //     }
  //     cerr << endl;
  //   }
}
