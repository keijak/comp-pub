#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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

i64 solve() {
  int h, w, a, b;
  cin >> h >> w >> a >> b;
  vector grid(h, vector(w, '_'));

  int ra = a, rb = b;
  auto f = [&](auto &f, int r, int c) -> i64 {
    if (r == h - 1 and c == w) {
      assert(ra == 0 and rb == 0);
      return 1;
    }
    if (c == w) return f(f, r + 1, 0);
    if (grid[r][c] != '_') {
      return f(f, r, c + 1);
    }
    i64 res = 0;
    if (rb > 0) {
      --rb;
      grid[r][c] = '#';
      res += f(f, r, c + 1);
      grid[r][c] = '_';
      ++rb;
    }
    if (ra > 0 and c < w - 1 and grid[r][c + 1] == '_') {
      --ra;
      grid[r][c] = grid[r][c + 1] = '#';
      res += f(f, r, c + 2);
      grid[r][c] = grid[r][c + 1] = '_';
      ++ra;
    }
    if (ra > 0 and r < h - 1 and grid[r + 1][c] == '_') {
      --ra;
      grid[r][c] = grid[r + 1][c] = '#';
      res += f(f, r, c + 1);
      grid[r][c] = grid[r + 1][c] = '_';
      ++ra;
    }
    return res;
  };
  return f(f, 0, 0);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
