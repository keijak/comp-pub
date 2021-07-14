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

template <typename T>
struct Compress {
  std::vector<T> values;

  explicit Compress(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

using namespace std;

i64 solve() {
  int H, W, n;
  cin >> W >> H >> n;
  vector<int> xs(n), ys(n);
  REP(i, n) { cin >> xs[i] >> ys[i]; }
  xs.push_back(0);
  xs.push_back(W + 1);
  ys.push_back(0);
  ys.push_back(H + 1);
  Compress<int> cx(xs), cy(ys);

  map<array<int, 4>, i64> memo;
  auto f = [&](auto &f, int x1, int y1, int x2, int y2) -> i64 {
    if (x1 >= x2 or y1 >= y2) return 0LL;
    array<int, 4> key = {x1, y1, x2, y2};
    if (auto mit = memo.find(key); mit != memo.end()) return mit->second;
    const i64 gain = (cx.value(x2) - cx.value(x1) - 1) +
                     (cy.value(y2) - cy.value(y1) - 1) - 1;
    i64 ans = 0;
    REP(i, n) {
      const int x = xs[i], y = ys[i];
      const int xi = cx.index(x), yi = cy.index(y);
      if (x1 < xi and xi < x2 and y1 < yi and yi < y2) {
        i64 r = gain;
        r += f(f, x1, y1, xi, yi);
        r += f(f, xi, y1, x2, yi);
        r += f(f, x1, yi, xi, y2);
        r += f(f, xi, yi, x2, y2);
        chmax(ans, r);
      }
    }
    memo[key] = ans;
    return ans;
  };
  return f(f, 0, 0, cx.size() - 1, cy.size() - 1);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
