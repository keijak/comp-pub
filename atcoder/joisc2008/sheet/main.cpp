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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, W, H;
  cin >> N >> W >> H;
  auto grid = make_vec(H, W, 0);
  REP(i, H) { cin >> grid[i]; }

  const int INF = 1e6;
  V<int> min_r(N, INF), min_c(N, INF), max_r(N, -INF), max_c(N, -INF);
  REP(r, H) REP(c, W) {
    int v = grid[r][c];
    if (v == 0) continue;
    v--;
    chmin(min_r[v], r);
    chmax(max_r[v], r);
    chmin(min_c[v], c);
    chmax(max_c[v], c);
  }

  V<set<int>> G(N);
  V<int> indeg(N);
  REP(i, N) {
    for (int r = min_r[i]; r <= max_r[i]; ++r) {
      for (int c = min_c[i]; c <= max_c[i]; ++c) {
        assert(grid[r][c] != 0);
        int v = grid[r][c] - 1;
        if (v != i) {
          if (G[i].insert(v).second) {
            indeg[v]++;
          }
        }
      }
    }
  }

  deque<int> q;
  REP(i, N) {
    if (indeg[i] == 0) {
      q.push_back(i);
    }
  }

  V<int> res;
  while (q.size()) {
    int v = q.front();
    q.pop_front();
    res.push_back(v + 1);
    for (auto u : G[v]) {
      if (--indeg[u] == 0) {
        q.push_back(u);
      }
    }
  }
  for (auto v : res) cout << ' ' << v;
  cout << endl;
}
