#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<vector<int>> g(n);
  vector<array<int, 2>> edges(n - 1);
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
    edges[i] = {a, b};
  }
  vector<int> depth(n, -1);

  auto dfs_depth = [&](auto self, int v, int p, int d) -> void {
    depth[v] = d;
    for (auto u : g[v]) {
      if (u == p) continue;
      self(self, u, v, d + 1);
    }
  };
  dfs_depth(dfs_depth, 0, -1, 0);
  DEBUG(depth);

  vector<i64> values(n, 0);

  int q;
  cin >> q;
  REP(qi, q) {
    i64 t, e, x;
    cin >> t >> e >> x;
    --e;
    auto [a, b] = edges[e];
    if (t == 1) {
      if (depth[a] > depth[b]) {
        values[a] += x;
      } else {
        values[0] += x;
        values[b] -= x;
      }
    } else {
      if (depth[a] < depth[b]) {
        values[b] += x;
      } else {
        values[0] += x;
        values[a] -= x;
      }
    }
  }

  auto dfs_prop = [&](auto self, int v, int p, i64 z) -> void {
    values[v] += z;
    for (auto u : g[v]) {
      if (u == p) continue;
      self(self, u, v, values[v]);
    }
  };
  dfs_prop(dfs_prop, 0, -1, 0LL);

  REP(i, n) { cout << values[i] << '\n'; }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
