#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
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

struct Cell {
  char ch;
  int below = 0;
  int l = 0;
  int r = 0;
};

i64 solve() {
  int n, m;
  cin >> n >> m;
  auto g = vector(n, vector(m, Cell{' '}));
  REP(i, n) { REP(j, m) cin >> g[i][j].ch; }

  REP(j, m) {
    for (int i = n - 1; i >= 0; --i) {
      auto &cell = g[i][j];
      if (cell.ch == '.') {
        cell.below = cell.l = cell.r = 0;
      } else {
        cell.below = 1;
        if (i < n - 1) {
          cell.below += g[i + 1][j].below;
        }
      }
    }
  }
  REP(j, m) {
    for (int i = n - 1; i >= 0; --i) {
      auto &cell = g[i][j];
      if (cell.ch == '.') continue;
      if (j == 0 or i == n - 1) {
        cell.l = 1;
      } else {
        cell.l = min(g[i + 1][j - 1].l + 1, cell.below);
      }
    }
  }
  for (int j = m - 1; j >= 0; --j) {
    for (int i = n - 1; i >= 0; --i) {
      auto &cell = g[i][j];
      if (cell.ch == '.') continue;
      if (j == m - 1 or i == n - 1) {
        cell.r = 1;
      } else {
        cell.r = min(g[i + 1][j + 1].r + 1, cell.below);
      }
    }
  }

  i64 ans = 0;
  REP(i, n) REP(j, m) {
    const auto &cell = g[i][j];
    ans += min(cell.l, cell.r);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << solve() << "\n"; }
}
