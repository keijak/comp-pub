#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
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
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

const array<int, 4> dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};

bool solve() {
  int n;
  cin >> n;
  vector<int> p(n);
  cin >> p;
  vector board(n, vector(n, -100));
  REP(i, n) { board[i][i] = -1; }
  REP(i, n) REP(j, i) { board[i][j] = -1; }

  // REP(i, n) DEBUG(board[i]);

  auto search = [&](const int gsize, MinHeap<pair<int, int>> &heap) -> bool {
    int rem = gsize;
    while (!heap.empty()) {
      assert(rem > 0);
      auto [y, x] = heap.top();
      heap.pop();
      if (board[x][y] != -1) continue;
      board[x][y] = gsize;
      --rem;
      //   DEBUG(gsize, x, y, rem);
      //   REP(i, n) DEBUG(board[i]);

      if (rem == 0) {
        // DEBUG(gsize, "OK");
        return true;
      }
      REP(i, 4) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 or ny < 0 or nx >= n or nx <= ny) {
          continue;
        }
        if (board[nx][ny] != -1) continue;
        // DEBUG(nx, ny, "push");
        heap.push({ny, nx});
      }
    }
    return false;
  };

  REP(i, n) {
    MinHeap<pair<int, int>> heap;
    heap.push({i, i});
    if (!search(p[i], heap)) return false;
  }

  REP(x, n) {
    REP(y, x + 1) {
      if (y > 0) cout << ' ';
      cout << board[x][y];
    }
    cout << '\n';
  }

  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  if (!solve()) {
    cout << -1 << "\n";
  }
}
