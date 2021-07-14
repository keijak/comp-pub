#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
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
using P = array<int, 2>;

const double EPS = 1e-9;

array<i64, 3> solve() {
  int n;
  cin >> n;
  vector<array<int, 2>> a(n);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    a[i] = {x, y};
  }
  i64 obtuse_count = 0;
  i64 right_count = 0;
  REP(i, n) {
    vector<pair<double, int>> ps;
    REP(j, n) {
      if (j == i) continue;
      double arg = atan2(a[j][1] - a[i][1], a[j][0] - a[i][0]);
      ps.emplace_back(arg, j);
    }
    sort(ALL(ps));
    vector<pair<double, int>> ps2(ps.size() * 2);
    REP(j, ssize(ps) * 2) {
      if (j < ssize(ps)) {
        ps2[j] = ps[j];
      } else {
        ps2[j] = ps[j % ssize(ps)];
        ps2[j].first += M_PI * 2.0;
      }
    }
    sort(ALL(ps2));
    int p = 0, q = 0, r = 0, s = 0;
    for (; p < n - 1; ++p) {
      chmax(q, p + 1);
      while (q < p + n - 1 and ps2[q].first < ps2[p].first + M_PI_2 - EPS) ++q;
      chmax(r, q);
      while (r < p + n - 1 and ps2[r].first <= ps2[p].first + M_PI_2 + EPS) ++r;
      chmax(s, r);
      while (s < p + n - 1 and ps2[s].first < ps2[p].first + M_PI - EPS) ++s;
      right_count += r - q;
      obtuse_count += s - r;
    }
  }

  i64 acute_count = i64(n) * (n - 1) * (n - 2) / 6 - right_count - obtuse_count;
  return {acute_count, right_count, obtuse_count};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  print_seq(solve());
}
