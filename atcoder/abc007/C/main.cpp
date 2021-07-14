#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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

#ifdef MY_DEBUG
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
  //   ios::sync_with_stdio(false);
  //   cin.tie(nullptr);

  int H, W;
  cin >> H >> W;
  int sy, sx, gy, gx;
  cin >> sy >> sx >> gy >> gx;
  sy--;
  sx--;
  gy--;
  gx--;
  V<string> grid(H);
  REP(i, H) { cin >> grid[i]; }

  array<int, 4> dy = {1, -1, 0, 0};
  array<int, 4> dx = {0, 0, 1, -1};

  auto dist = vector(H, vector(W, optional<int>()));
  dist[sy][sx] = 0;
  deque<tuple<int, int, int>> q;
  q.emplace_back(0, sy, sx);
  while (q.size()) {
    auto [d, y, x] = q.front();
    q.pop_front();
    assert(d == dist[y][x].value());
    if (y == gy and x == gx) break;
    REP(i, 4) {
      int ny = y + dy[i];
      int nx = x + dx[i];
      if (ny < 0 or ny >= H or nx < 0 or nx >= W) continue;
      if (grid[ny][nx] == '#') continue;
      if (dist[ny][nx].has_value()) continue;
      dist[ny][nx] = d + 1;
      q.emplace_back(d + 1, ny, nx);
    }
  }

  assert(dist[gy][gx].has_value());
  cout << dist[gy][gx].value() << endl;
}
