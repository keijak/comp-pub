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

struct Edge {
  int to;
  int cost;
};

const i64 INF = 1e18;
template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, K;
  cin >> N >> M >> K;
  V<V<Edge>> G(N);
  REP(i, M) {
    int a, b, l;
    cin >> a >> b >> l;
    a--;
    b--;
    G[a].push_back({b, l});
    G[b].push_back({a, l});
  }
  V<int> shops(K);
  REP(i, K) {
    cin >> shops[i];
    shops[i]--;
  }

  V<i64> mindist(N, INF);
  MinHeap<tuple<i64, int>> heap;
  REP(i, K) {
    mindist[shops[i]] = 0;
    heap.emplace(0, shops[i]);
  }
  while (heap.size()) {
    auto [d, cur] = heap.top();
    heap.pop();
    if (d > mindist[cur]) continue;
    for (const Edge &e : G[cur]) {
      i64 nd = d + e.cost;
      if (mindist[e.to] > nd) {
        mindist[e.to] = nd;
        heap.emplace(nd, e.to);
      }
    }
  }

  i64 ans = 0;
  REP(i, N) {
    for (const auto &e : G[i]) {
      i64 d1 = min(mindist[i], mindist[e.to]);
      i64 d2 = max(mindist[i], mindist[e.to]);
      if (d1 + e.cost == d2) {
        chmax(ans, d2);
      } else {
        i64 x = d1 + e.cost - d2;
        x = (x + 1) / 2;
        chmax(ans, d2 + x);
      }
    }
  }
  cout << ans << endl;
}
