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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const int L = 1000;
  int N, M, D, K;
  cin >> N >> M >> D >> K;
  const int D2 = D * D;

  V<map<int, int>> pos(L);
  REP(i, N) {
    int x, y;
    cin >> x >> y;
    pos[y][x] = i;
  }

  V<V<int>> G(N);
  REP(y, L) {
    const auto &py = pos[y];
    for (auto [x, j] : py) {
      auto it = py.find(x);
      auto ub = py.upper_bound(x + D);
      for (++it; it != ub; ++it) {
        int k = it->second;
        G[j].push_back(k);
        G[k].push_back(j);
      }
      for (int dy = 1; dy <= D; ++dy) {
        if (y + dy >= L) break;
        const auto &pn = pos[y + dy];
        int dy2 = dy * dy;
        auto itx = pn.lower_bound(x);
        for (auto it = itx; it != pn.end(); ++it) {
          int dx = it->first - x;
          if (dx * dx + dy2 > D2) break;
          int k = it->second;
          G[j].push_back(k);
          G[k].push_back(j);
        }
        while (itx != pn.begin()) {
          --itx;
          int dx = x - itx->first;
          if (dx * dx + dy2 > D2) break;
          int k = itx->second;
          G[j].push_back(k);
          G[k].push_back(j);
        }
      }
    }
  }

  // BFS
  const int INF = 1e9;
  V<int> start_date(N, -INF);
  start_date[0] = 0;
  deque<tuple<int, int>> q;
  q.emplace_back(0, 0);
  while (q.size()) {
    auto [sd, cur] = q.front();
    q.pop_front();
    if (sd > K) break;
    for (auto nex : G[cur]) {
      if (start_date[nex] != -INF) continue;
      start_date[nex] = sd + 1;
      q.emplace_back(sd + 1, nex);
    }
  }
  int count = 0;
  for (auto d : start_date) {
    if (d <= K and K < d + M) {
      ++count;
    }
  }
  cout << count << endl;
}
