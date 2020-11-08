#include <bits/stdc++.h>

#include <atcoder/dsu>
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  V<string> grid(n);
  int num_cells = 0;
  REP(i, n) {
    cin >> grid[i];
    REP(j, m) {
      if (grid[i][j] == '.') ++num_cells;
    }
  }
  array<int, 4> dx = {1, -1, 0, 0}, dy = {0, 0, 1, -1};

  auto bfs = [&](int r0, int c0) -> bool {
    V<V<bool>> visited(n, V<bool>(m, false));
    visited[r0][c0] = true;
    deque<tuple<int, int>> q;
    q.emplace_back(r0, c0);
    int vis_count = 0;
    while (q.size()) {
      auto [r, c] = q.front();
      q.pop_front();
      REP(i, 4) {
        int nr = r + dy[i];
        int nc = c + dx[i];
        if (nr < 0 or nr >= n or nc < 0 or nc >= m) continue;
        if (grid[nr][nc] == '#') continue;
        if (visited[nr][nc]) continue;
        visited[nr][nc] = true;
        vis_count++;
        q.emplace_back(nr, nc);
      }
    }
    return vis_count == num_cells;
  };

  int ans = 0;
  REP(i, n) {
    REP(j, m) {
      if (grid[i][j] != '#') continue;
      if (bfs(i, j)) {
        ++ans;
      }
    }
  }
  cout << ans << endl;
}
