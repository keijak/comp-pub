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

struct Cell {
  bool actor;
  bool l, r, u, d;
};
i64 solve() {
  int n, m;
  cin >> n >> m;
  auto plan = vector(n, vector(m, Cell{0, 0, 0, 0, 0}));
  REP(i, n) REP(j, m) cin >> plan[i][j].actor;

  REP(i, n) {
    REP(j, m) {
      if (j - 1 >= 0) {
        plan[i][j].l = plan[i][j - 1].l or plan[i][j - 1].actor;
      }
      int rj = m - 1 - j;
      if (rj + 1 < m) {
        plan[i][rj].r = plan[i][rj + 1].r or plan[i][rj + 1].actor;
      }

      if (i - 1 >= 0) {
        plan[i][j].u = plan[i - 1][j].u or plan[i - 1][j].actor;
      }
      int ri = n - 1 - i;
      if (ri + 1 < n) {
        plan[ri][j].d = plan[ri + 1][j].d or plan[ri + 1][j].actor;
      }
    }
  }

  i64 ans = 0;
  REP(i, n) {
    REP(j, m) {
      if (plan[i][j].actor) continue;
      if (plan[i][j].l) ++ans;
      if (plan[i][j].r) ++ans;
      if (plan[i][j].u) ++ans;
      if (plan[i][j].d) ++ans;
    }
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
