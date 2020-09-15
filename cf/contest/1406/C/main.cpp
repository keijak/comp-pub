#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define WHOLE(x) std::begin(x), std::end(x)
#define LEN(a) int((a).size())

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
// Simple Graph
struct Graph {
  int n;
  V<V<int>> adj;
  explicit Graph(int n) : n(n), adj(n) {}
  void input_undirected(int m) { input(m, false); }
  void input_directed(int m) { input(m, true); }

 private:
  void input(int m, bool is_directed) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      cin >> u >> v;
      u--, v--;  // to 0-indexed.
      adj[u].push_back(v);
      if (not is_directed) adj[v].push_back(u);
    }
  }
};

void solve() {
  int n;
  cin >> n;
  Graph g(n);
  g.input_undirected(n - 1);
  int min_comp = n;
  V<pair<int, int>> centroids;
  centroids.reserve(n);
  auto dfs1 = [&](auto self, int v, int p) -> int {
    int num_max = 0;
    int child_max = 0;
    int sub_size = 1;
    for (auto u : g.adj[v]) {
      if (u == p) continue;
      int s = self(self, u, v);
      sub_size += s;
      if (child_max < s) {
        child_max = s;
        num_max = 1;
      } else if (child_max == s) {
        num_max++;
      }
    }
    if (child_max < n - sub_size) {
      child_max = n - sub_size;
      num_max = 1;
    } else if (child_max == n - sub_size) {
      num_max++;
    }
    if (min_comp > child_max) {
      min_comp = child_max;
      centroids.clear();
      centroids.push_back({num_max, v});
      DEBUG(min_comp, v);
    } else if (min_comp == child_max) {
      DEBUG(min_comp, v);
      centroids.push_back({num_max, v});
    }
    return sub_size;
  };
  dfs1(dfs1, 0, -1);
  assert(min_comp >= 1);
  if (centroids.size() == 1) {
    REP(i, n) {
      for (auto u : g.adj[i]) {
        cout << (i + 1) << ' ' << (u + 1) << '\n';
        cout << (i + 1) << ' ' << (u + 1) << '\n';
        return;
      }
    }
  }
  sort(WHOLE(centroids));

  V<int> sizes(n);
  auto dfs2 = [&](auto self, int v, int p) -> int {
    int sub_size = 1;
    for (auto u : g.adj[v]) {
      if (u == p) continue;
      int s = self(self, u, v);
      sub_size += s;
    }
    return sizes[v] = sub_size;
  };
  int z = centroids[0].second;
  dfs2(dfs2, z, -1);
  DEBUG(z, min_comp);
  DEBUG(sizes);

  for (auto v : g.adj[z]) {
    if (sizes[v] != min_comp) continue;
    for (auto u : g.adj[v]) {
      DEBUG(v, u);
      if (u == z) continue;
      cout << (v + 1) << ' ' << (u + 1) << '\n';
      cout << (z + 1) << ' ' << (u + 1) << '\n';
      return;
    }
  }
  assert(false);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DEBUG(i, t);
    solve();
  }
}
