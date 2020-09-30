#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
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
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
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
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  i64 x0, y0, ax, ay, bx, by, xs, ys, t;
  cin >> x0 >> y0 >> ax >> ay >> bx >> by;
  cin >> xs >> ys >> t;

  V<i64> X, Y;
  X.push_back(x0);
  Y.push_back(y0);
  i64 dmin = abs(x0 - xs) + abs(y0 - ys);
  for (int i = 1;; ++i) {
    i64 x = ax * X.back() + bx;
    i64 y = ay * Y.back() + by;
    X.push_back(x);
    Y.push_back(y);
    i64 d = abs(x - xs) + abs(y - ys);
    if (not chmin(dmin, d) and d > t) break;
  }
  int M = X.size();

  int ans = 0;  // maximize
  REP(i, M) {
    // landing point.
    i64 d = abs(X[i] - xs) + abs(Y[i] - ys);
    if (d > t) continue;
    chmax(ans, 1);  // landing
    i64 remt = t - d;

    for (int j = i; j >= 0; --j) {
      i64 dj = abs(X[i] - X[j]) + abs(Y[i] - Y[j]);
      if (dj > remt) break;
      int left_count = i - j + 1;
      chmax(ans, left_count);
      if (2 * dj > remt) continue;
      i64 remt2 = remt - 2 * dj;
      for (int k = i + 1; k < M; ++k) {
        i64 dk = abs(X[k] - X[i]) + abs(Y[k] - Y[i]);
        if (dk > remt2) break;
        chmax(ans, left_count + k - i);
      }
    }

    for (int j = i; j < M; ++j) {
      i64 dj = abs(X[i] - X[j]) + abs(Y[i] - Y[j]);
      if (dj > remt) break;
      int right_count = j - i + 1;
      chmax(ans, right_count);
      if (2 * dj > remt) continue;
      i64 remt2 = remt - 2 * dj;
      for (int k = i - 1; k >= 0; ++k) {
        i64 dk = abs(X[k] - X[i]) + abs(Y[k] - Y[i]);
        if (dk > remt2) break;
        chmax(ans, right_count + i - k);
      }
    }
  }
  cout << ans << endl;
}
