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

struct Node {
  int height;
  int count;
  int parent;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  V<int> a(n);
  REP(i, n) { cin >> a[i]; }

  V<Node> nodes;
  nodes.push_back({0, 0, -1});
  V<V<int>> edges;
  edges.push_back({});
  deque<int> stk;
  stk.push_back(0);
  for (int i = 0; i < n; ++i) {
    while (nodes[stk.back()].height > a[i]) {
      stk.pop_back();
    }
    if (nodes[stk.back()].height < a[i]) {
      int parent = stk.back();
      int node_id = nodes.size();
      nodes.push_back({a[i], 1, parent});
      edges.push_back({});
      edges[parent].push_back(node_id);
      stk.push_back(node_id);
    } else {
      int node_id = stk.back();
      assert(a[i] == nodes[node_id].height);
      nodes[node_id].count++;
    }
  }

  auto dfs = [&](auto self, int v, i64 acc) -> i64 {
    i64 h = nodes[v].height;
    i64 c = nodes[v].count;
    i64 res0 = c - (h - acc);
    i64 res = res0;
    for (auto u : edges[v]) {
      res += self(self, u, h);
    }
    DEBUG(v, h, c, res, res0);
    return max(res, 0LL);
  };

  i64 save = dfs(dfs, 0, 0);
  i64 ans = n - save;
  DEBUG(ans, n, save);
  cout << max(ans, 0LL) << endl;
}
