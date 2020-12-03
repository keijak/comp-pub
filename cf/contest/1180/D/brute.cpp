#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
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

void solve() {
  i64 n, m;
  cin >> n >> m;
  set<pair<int, int>> deltas;
  vector<pair<int, int>> res;
  auto visited = vector(n, vector(m, false));
  auto dfs = [&](auto self, int x, int y) -> bool {
    if (ssize(res) == n * m) return true;
    REP(i, n) REP(j, m) {
      if (visited[i][j]) continue;
      int dx = i - x;
      int dy = j - y;
      if (deltas.count({dx, dy})) continue;
      visited[i][j] = true;
      deltas.emplace(dx, dy);
      res.emplace_back(i, j);
      bool ok = self(self, i, j);
      if (ok) return true;
      res.pop_back();
      deltas.erase(pair{dx, dy});
      visited[i][j] = false;
    }
    return false;
  };
  res.emplace_back(0, 0);
  visited[0][0] = true;
  auto possible = dfs(dfs, 0, 0);

  if (not possible) {
    cout << -1 << '\n';
    return;
  }
  for (auto [a, b] : res) {
    cout << (a + 1) << ' ' << (b + 1) << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
