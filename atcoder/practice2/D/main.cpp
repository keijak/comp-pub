#include <bits/stdc++.h>

#include <atcoder/maxflow>
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<string> grid(n);
  REP(i, n) { cin >> grid[i]; }

  atcoder::mf_graph<int> g(n * m + 2);
  int source = n * m;
  int sink = n * m + 1;
  array<int, 4> dy = {0, 0, 1, -1};
  array<int, 4> dx = {1, -1, 0, 0};

  REP(i, n) {
    REP(j, m) {
      if (grid[i][j] == '#') continue;

      if ((i + j) % 2 == 1) {
        g.add_edge(i * m + j, sink, 1);
        continue;
      }

      g.add_edge(source, i * m + j, 1);

      REP(k, 4) {
        int ni = i + dy[k];
        int nj = j + dx[k];
        if (ni < 0 or ni >= n or nj < 0 or nj >= m) continue;
        if (grid[ni][nj] == '#') continue;
        g.add_edge(i * m + j, ni * m + nj, 1);
      }
    }
  }

  int f = g.flow(source, sink);
  cout << f << '\n';

  for (const auto &e : g.edges()) {
    if (e.flow == 0) continue;
    if (e.from == source or e.to == sink) continue;
    int i1 = e.from / m, j1 = e.from % m;
    int i2 = e.to / m, j2 = e.to % m;
    assert((i1 == i2) ^ (j1 == j2));
    if (i1 == i2) {
      grid[i1][min(j1, j2)] = '>';
      grid[i1][max(j1, j2)] = '<';
    } else {
      grid[min(i1, i2)][j1] = 'v';
      grid[max(i1, i2)][j1] = '^';
    }
  }

  REP(i, n) { cout << grid[i] << '\n'; }
}
