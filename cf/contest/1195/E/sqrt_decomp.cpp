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

// Returns ceil(sqrt(x)).
i64 ceilsqrt(i64 x) {
  assert(x >= 0);
  if (x <= 1) return x;
  i64 r = std::ceil(sqrtl((long double)x));
  while (r * r > x) --r;
  while (r * r < x) ++r;
  return r;
}

const int BIG = 1e9 + 5000;

i64 solve() {
  i64 n, m, a, b, g0, x, y, z;
  cin >> n >> m >> a >> b;
  cin >> g0 >> x >> y >> z;
  vector h(n, vector(m, 0));
  h[0][0] = g0;
  {
    i64 g = g0;
    REP(i, 1, n * m) {
      g = (g * x + y) % z;
      int r = i / m;
      int c = i % m;
      h[r][c] = g;
    }
  }
  const i64 bn = ceilsqrt(n), bm = ceilsqrt(m);
  vector mins(bn, vector(bm, BIG));
  REP(i, n) {
    int fi = i / bn;
    REP(j, m) {
      int fj = j / bm;
      chmin(mins[fi][fj], h[i][j]);
    }
  }

  i64 ans = 0;
  REP(i, n - a + 1) {
    const int fi = i / bn;
    const int ei = (i + a) / bn;
    REP(j, m - b + 1) {
      const int fj = j / bm;
      const int ej = (j + b) / bm;
      i64 val = BIG;

      for (int bx = fi + 1; bx < ei; ++bx) {
        for (int by = fj + 1; by < ej; ++by) {
          chmin(val, mins[bx][by]);
        }
      }

      for (int x = i; x < i + a; ++x) {
        for (int y = j;; ++y) {
          if (x >= (fi + 1) * bn and x < ei * bn and y == (fj + 1) * bm) {
            y = ej * bm;
          }
          if (y >= j + b) break;
          chmin(val, h[x][y]);
        }
      }
      ans += val;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
