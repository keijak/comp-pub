#include <bits/stdc++.h>

#include <atcoder/mincostflow>

using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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

const i64 INF = 1e9;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;

  auto A = vector(N, vector(N, 0LL));
  i64 total = 0;
  i64 amax = 0;
  REP(i, N) {
    REP(j, N) {
      cin >> A[i][j];
      total += A[i][j];
      chmax(amax, A[i][j]);
    }
  }

  int source = 0;
  int sink = 1;
  int row_offset = 2;
  int col_offset = row_offset + N;
  int num_nodes = col_offset + N;
  atcoder::mcf_graph<int, i64> g(num_nodes);
  g.add_edge(source, sink, N * K, INF);

  REP(i, N) {
    g.add_edge(source, row_offset + i, K, 0);
    g.add_edge(col_offset + i, sink, K, 0);
  }
  REP(i, N) {
    REP(j, N) { g.add_edge(row_offset + i, col_offset + j, 1, INF - A[i][j]); }
  }
  auto [maxflow, mincost] = g.flow(source, sink, N * K);
  i64 maxgain = N * K * INF - mincost;
  cout << maxgain << endl;

  vector<string> grid(N, string(N, '.'));
  auto edges = g.edges();
  for (auto e : edges) {
    if (e.from == source || e.to == sink || e.flow == 0) continue;
    grid[e.from - row_offset][e.to - col_offset] = 'X';
  }
  for (int i = 0; i < N; i++) {
    cout << grid[i] << endl;
  }
}