#include <bits/stdc++.h>
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

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<string> grid(n);
  unordered_set<int> walls;
  set<int> pieces;
  int piece_cnt = 0;
  REP(i, n) {
    cin >> grid[i];
    REP(j, m) {
      int pos = i * m + j;
      if (grid[i][j] == 'o') {
        ++piece_cnt;
        pieces.insert(pos);
      } else if (grid[i][j] == '#') {
        walls.insert(pos);
      }
    }
  }

  array<int, 2> dx = {1, 0};
  array<int, 2> dy = {0, 1};
  map<vector<int>, int> memo;

  auto dfs = [&](auto self) -> int {
    vector<int> curs(ALL(pieces));
    auto it = memo.find(curs);
    if (it != memo.end()) return it->second;

    int res = 0;
    for (int cur : curs) {
      int r = cur / m;
      int c = cur % m;
      REP(j, 2) {
        int nr = r + dx[j];
        int nc = c + dy[j];
        if (nr == n or nc == m) continue;
        int ns = nr * m + nc;
        if (walls.count(ns)) continue;
        if (pieces.count(ns)) continue;

        pieces.erase(cur);
        pieces.insert(ns);
        chmax(res, self(self) + 1);
        pieces.erase(ns);
        pieces.insert(cur);
      }
    }
    memo[curs] = res;
    return res;
  };
  return dfs(dfs);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
