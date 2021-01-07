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

struct Edge {
  int to;
  int visited;
  Edge() : to(-1), visited(-1) {}
  explicit Edge(int to, int vis = -1) : to(to), visited(vis) {}
};
bool operator<(const Edge &x, const Edge &y) { return x.visited < y.visited; }

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].emplace_back(b);
    g[b].emplace_back(a);
  }

  int q;
  cin >> q;
  vector<array<int, 3>> queries(q);
  REP(qi, q) {
    int v, d, c;
    cin >> v >> d >> c;
    --v;
    queries[qi] = {v, d, c};
  }
  reverse(ALL(queries));

  vector<int> color(n, -1);
  auto paint = [&](auto rec, int v, int c, int d, int par) -> void {
    assert(d >= 0);
    if (color[v] == -1) {
      color[v] = c;
    }
    if (d == 0) return;
    --d;
    if (g[v].empty() or d <= g[v][0].visited) return;
    vector<Edge> links = g[v];
    map<int, int> renew;
    for (const auto &e : links) {
      if (e.to == par) continue;
      if (d <= e.visited) continue;
      if (renew.count(e.to)) {
        chmax(renew[e.to], d);
      } else {
        renew[e.to] = d;
      }
      rec(rec, e.to, c, d, v);
    }
    if (not renew.empty()) {
      for (auto &e : g[v]) {
        if (renew.count(e.to)) {
          e.visited = renew[e.to];
        }
      }
      sort(ALL(g[v]));
    }
  };
  for (auto [v, d, c] : queries) {
    paint(paint, v, c, d, -1);
  }

  REP(i, n) {
    if (color[i] == -1) {
      color[i] = 0;
    }
    cout << color[i] << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
