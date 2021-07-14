#include <bits/stdc++.h>

#include <atcoder/mincostflow>

#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
const int INF = 1e9;
const i64 INF64 = 1e18;

i64 solve() {
  int n, m;
  cin >> n >> m;
  const int nm = n * m;

  vector<string> grid(n);
  set<int> pieces;
  int piece_cnt = 0;
  i64 init_cost = 0;
  REP(i, n) {
    cin >> grid[i];
    REP(j, m) {
      int pos = i * m + j;
      if (grid[i][j] == 'o') {
        ++piece_cnt;
        pieces.insert(pos);
        init_cost += (n - 1 - i) + (m - 1 - j);
      }
    }
  }

  atcoder::mcf_graph<int, int> g(nm + 2);
  const int SOURCE = nm;
  const int SINK = nm + 1;
  for (auto p : pieces) {
    g.add_edge(SOURCE, p, 1, 0);
  }
  REP(i, n) REP(j, m) {
    if (grid[i][j] == '#') continue;
    int pos = i * m + j;
    int cost = (n - 1 - i) + (m - 1 - j);
    g.add_edge(pos, SINK, 1, cost);
  }
  REP(i, n) REP(j, m) {
    if (grid[i][j] == '#') continue;
    int pos = i * m + j;
    if (i < n - 1 and grid[i + 1][j] != '#') {
      int pos2 = (i + 1) * m + j;
      g.add_edge(pos, pos2, piece_cnt, 0);
    }
    if (j < m - 1 and grid[i][j + 1] != '#') {
      int pos2 = i * m + (j + 1);
      g.add_edge(pos, pos2, piece_cnt, 0);
    }
  }
  auto [flow, mincost] = g.flow(SOURCE, SINK);
  DEBUG(flow, piece_cnt);
  assert(flow == piece_cnt);
  DEBUG(mincost);
  return init_cost - mincost;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
