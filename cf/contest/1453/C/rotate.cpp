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

template <typename T>
void rotate90(vector<vector<T>> &grid) {
  int n = grid.size();
  if (n == 0) return;
  int m = grid[0].size();
  if (m == 0) return;
  auto tmp = vector(m, vector(n, grid[0][0]));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      tmp[m - 1 - j][i] = grid[i][j];
    }
  }
  grid = std::move(tmp);
}

vector<i64> solve() {
  int n;
  cin >> n;
  auto board = vector(n, vector(n, 0));
  REP(i, n) REP(j, n) {
    char d;
    cin >> d;
    board[i][j] = d - '0';
  }

  vector<i64> res(10);
  REP(rotation, 4) {
    vector<int> maxr(10, -1), minr(10, n);
    REP(r, n) REP(c, n) {
      int d = board[r][c];
      chmax(maxr[d], r);
      chmin(minr[d], r);
    }

    for (int d = 0; d < 10; ++d) {
      i64 area = 0;
      REP(r, n) {
        int mx = -1;
        int mn = -1;
        REP(c, n) {
          if (board[r][c] == d) {
            if (mn == -1) mn = c;
            chmax(mx, c);
          }
        }
        if (mn == -1) continue;
        i64 w = mx - mn;
        i64 h = max(r, n - 1 - r);
        chmax(area, w * h);
      }
      chmax(res[d], area);
    }

    REP(r, n) REP(c, n) {
      int d = board[r][c];
      i64 w = max(c, n - 1 - c);
      i64 h = max(maxr[d] - r, r - minr[d]);
      chmax(res[d], w * h);
    }

    rotate90(board);
  }

  return res;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { pprint(solve()); }
}
