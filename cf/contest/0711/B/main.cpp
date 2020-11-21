#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

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
  int n;
  cin >> n;
  auto grid = vector(n, vector(n, 0LL));
  REP(i, n) REP(j, n) cin >> grid[i][j];

  if (n == 1) return 1;
  set<i64> s1, s2;

  REP(i, n) {
    i64 s = 0;
    bool slot = false;
    REP(j, n) {
      if (grid[i][j] == 0) slot = true;
      s += grid[i][j];
    }
    if (slot) {
      s1.insert(s);
    } else {
      s2.insert(s);
    }
  }
  REP(j, n) {
    i64 s = 0;
    bool slot = false;
    REP(i, n) {
      if (grid[i][j] == 0) slot = true;
      s += grid[i][j];
    }
    if (slot) {
      s1.insert(s);
    } else {
      s2.insert(s);
    }
  }
  {
    int i = 0, j = 0;
    i64 s = 0;
    bool slot = false;
    REP(d, n) {
      if (grid[i][j] == 0) slot = true;
      s += grid[i][j];
      ++i, ++j;
    }
    if (slot) {
      s1.insert(s);
    } else {
      s2.insert(s);
    }
  }
  {
    int i = 0, j = n - 1;
    i64 s = 0;
    bool slot = false;
    REP(d, n) {
      if (grid[i][j] == 0) slot = true;
      s += grid[i][j];
      ++i, --j;
    }
    if (slot) {
      s1.insert(s);
    } else {
      s2.insert(s);
    }
  }

  if (ssize(s1) != 1 or ssize(s2) != 1) {
    return -1;
  }
  i64 res = *s2.begin() - *s1.begin();
  if (res <= 0) return -1;
  return res;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << '\n';
}