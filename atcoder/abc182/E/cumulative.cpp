#include <bits/stdc++.h>
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
  int H, W, N, M;
  cin >> H >> W >> N >> M;
  V<string> grid(H + 2);
  grid[0] = grid[H + 1] = string(W + 2, '#');
  REP(i, H) {
    grid[i + 1] = string(W + 2, '.');
    grid[i + 1][0] = grid[i + 1][W + 1] = '#';
  }

  V<pair<int, int>> lights(N), blocks(M);
  REP(i, N) {
    int a, b;
    cin >> a >> b;
    lights[i] = {a, b};
    assert(grid[a][b] == '.');
    grid[a][b] = '*';
  }
  REP(i, M) {
    int a, b;
    cin >> a >> b;
    blocks[i] = {a, b};
    assert(grid[a][b] == '.');
    grid[a][b] = '#';
  }

  auto path = vector(H + 2, vector(W + 2, vector(4, false)));
  REP(r, H + 2) {
    REP(c, W + 1) {
      if ((grid[r][c] == '*' or path[r][c][0]) and grid[r][c + 1] != '#') {
        path[r][c + 1][0] = true;
      }
    }
    REP(c, W + 1) {
      int rc = W + 1 - c;
      if ((grid[r][rc] == '*' or path[r][rc][1]) and grid[r][rc - 1] != '#') {
        path[r][rc - 1][1] = true;
      }
    }
  }
  REP(c, W + 2) {
    REP(r, H + 1) {
      if ((grid[r][c] == '*' or path[r][c][2]) and grid[r + 1][c] != '#') {
        path[r + 1][c][2] = true;
      }
    }
    REP(r, H + 1) {
      int rr = H + 1 - r;
      if ((grid[rr][c] == '*' or path[rr][c][3]) and grid[rr - 1][c] != '#') {
        path[rr - 1][c][3] = true;
      }
    }
  }

  int cnt = 0;
  REP(r, H + 2) {
    REP(c, W + 2) {
      if (grid[r][c] == '*' or path[r][c][0] or path[r][c][1] or
          path[r][c][2] or path[r][c][3]) {
        ++cnt;
      }
    }
  }
  cout << cnt << endl;
  /*
    REP(i, H + 2) {
      REP(j, W + 2) {
        if (grid[i][j] == '*' or grid[i][j] == '#') {
          cout << grid[i][j];
        } else if (path[i][j][0]) {
          cout << '>';
        } else if (path[i][j][1]) {
          cout << '<';
        } else if (path[i][j][2]) {
          cout << 'v';
        } else {
          cout << ' ';
        }
      }
      cout << endl;
    }
    */
}
