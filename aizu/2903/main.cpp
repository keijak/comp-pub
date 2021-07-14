#include <bits/stdc++.h>

#include <atcoder/maxflow>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
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

i64 solve() {
  int R, C;
  cin >> R >> C;
  vector<string> grid(R);
  vector<vector<int>> node_id(R, vector<int>(C, -1));
  int n = 0;
  REP(i, R) {
    cin >> grid[i];
    assert(ssize(grid[i]) == C);
    REP(j, C) {
      if (grid[i][j] == '#') {
        node_id[i][j] = n++;
      }
    }
  }

  vector<pair<int, int>> hor, ver;
  REP(i, R) REP(j, C) {
    if (grid[i][j] != '#') continue;
    const int v = node_id[i][j];
    if (j + 1 < C and grid[i][j + 1] == '#') {
      const int u = node_id[i][j + 1];
      hor.emplace_back(v, u);
    }
    if (i + 1 < R and grid[i + 1][j] == '#') {
      const int u = node_id[i + 1][j];
      ver.emplace_back(v, u);
    }
  }

  const i64 BIG = 1000;
  const int nh = ssize(hor), nv = ssize(ver);
  atcoder::mf_graph<i64> g(n + nh + nv + 2);
  const int source = n + nh + nv, sink = n + nh + nv + 1;
  REP(i, n) {
    g.add_edge(source, i, 1);
    g.add_edge(i, sink, 1);
  }
  i64 total_cost = -nh - nv;
  REP(i, nh) {
    const int x = n + i;
    auto [u, v] = hor[i];
    g.add_edge(source, x, 1);
    g.add_edge(x, u, BIG);
    g.add_edge(x, v, BIG);
  }
  REP(i, nv) {
    const int x = n + nh + i;
    auto [u, v] = ver[i];
    g.add_edge(x, sink, 1);
    g.add_edge(u, x, BIG);
    g.add_edge(v, x, BIG);
  }
  i64 res = g.flow(source, sink);
  total_cost += res;
  DEBUG(res, total_cost);
  return total_cost;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
