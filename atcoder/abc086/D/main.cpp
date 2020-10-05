#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
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

  int n, k;
  cin >> n >> k;
  const int k2 = k * 2;
  auto grid = vector(2, vector(k2, vector(k2, 0LL)));
  REP(i, n) {
    int x, y;
    char color;
    cin >> x >> y >> color;
    grid[color == 'W'][x % k2][y % k2]++;
  }

  auto cum = vector(2, vector(k2 + 1, vector(k2 + 1, 0LL)));
  REP(c, 2) REP(i, k2) REP(j, k2) {
    cum[c][i + 1][j + 1] =
        cum[c][i + 1][j] + cum[c][i][j + 1] - cum[c][i][j] + grid[c][i][j];
  }

  auto sum = [&](int w, int x, int y) -> i64 {
    int x2 = clamp(x + k, 0, k2);
    int y2 = clamp(y + k, 0, k2);
    x = clamp(x, 0, k2);
    y = clamp(y, 0, k2);
    return cum[w][x2][y2] - cum[w][x2][y] - cum[w][x][y2] + cum[w][x][y];
  };

  i64 maxhope = 0;
  for (int x = 0; x < k2; ++x) {
    for (int y = 0; y < k2; ++y) {
      i64 hope = 0;
      for (int dx = -2; dx < 2; ++dx) {
        for (int dy = -2; dy < 2; ++dy) {
          if ((dx + dy) % 2 == 0) {
            hope += sum(0, x + dx * k, y + dy * k);
          } else {
            hope += sum(1, x + dx * k, y + dy * k);
          }
        }
      }
      chmax(maxhope, hope);
    }
  }

  cout << maxhope << endl;
}
