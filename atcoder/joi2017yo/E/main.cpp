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
ostream &operator<<(ostream &os, const deque<T> &a) {
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int H, W;
  cin >> H >> W;
  auto alt = make_vec(H, W, 0);
  REP(row, H) {
    REP(col, W) { cin >> alt[row][col]; }
  }

  array<int, 4> dx = {0, 0, 1, -1};
  array<int, 4> dy = {1, -1, 0, 0};
  auto indeg = make_vec(H, W, 0);
  deque<pair<int, int>> q;
  V<V<set<int>>> dests(H, V<set<int>>(W));
  REP(row, H) {
    REP(col, W) {
      REP(i, 4) {
        int nr = row + dy[i];
        int nc = col + dx[i];
        if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
        if (alt[nr][nc] < alt[row][col]) {
          indeg[row][col]++;
        }
      }
      if (indeg[row][col] == 0) {
        q.emplace_back(row, col);
        dests[row][col].insert(row * W + col);
      }
    }
  }

  while (q.size()) {
    auto [row, col] = q.front();
    q.pop_front();
    REP(i, 4) {
      int nr = row + dy[i];
      int nc = col + dx[i];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (alt[nr][nc] > alt[row][col]) {
        for (auto d : dests[row][col]) {
          auto &dd = dests[nr][nc];
          if (SZ(dd) < 2) dd.insert(d);
        }

        assert(indeg[nr][nc] > 0);
        if (--indeg[nr][nc] == 0) {
          q.emplace_back(nr, nc);
        }
      }
    }
  }

  int ans = 0;
  REP(row, H) {
    REP(col, W) {
      if (SZ(dests[row][col]) >= 2) {
        ++ans;
      }
    }
  }
  cout << ans << endl;
}
