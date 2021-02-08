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
                        std::ostream *os = nullptr) {
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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
  vector<int> a(n);
  cin >> a;
  vector<pair<int, int>> rl;
  REP(i, n) {
    if (i > 0 and a[i] == a[i - 1]) {
      rl.back().second++;
    } else {
      rl.emplace_back(a[i], 1);
    }
  }
  DEBUG(rl);
  int m = ssize(rl);
  if (m <= 3) {
    return n;
  }

  i64 ans = 0;
  REP(i, m - 1) {
    if (rl[i].first == 2) {
      // 1 [2 1] 2 => 1 [1 2] 2
      i64 s = rl[i].second + rl[i + 1].second;
      if (i - 1 >= 0) s += rl[i - 1].second;
      if (i + 2 < m) s += rl[i + 2].second;
      chmax(ans, s);
    }
  }
  i64 max_ones = 0;
  REP(i, m) {
    i64 s = rl[i].second + max_ones;
    if (i + 1 < m) s += rl[i + 1].second;
    chmax(ans, s);
    if (rl[i].first == 1) {
      chmax(max_ones, rl[i].second);
    }
  }
  i64 max_twos = 0;
  for (int i = m - 1; i >= 0; --i) {
    i64 s = rl[i].second + max_twos;
    if (i - 1 >= 0) s += rl[i - 1].second;
    chmax(ans, s);
    if (rl[i].first == 2) {
      chmax(max_twos, rl[i].second);
    }
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
