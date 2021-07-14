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

const i64 BIG = 1e9;

struct segtree {
  int H, W;
  vector<vector<i64>> dat;
  segtree() {}
  segtree(vector<vector<i64>> &f) {
    H = W = 1;
    while (H < (int)f.size()) H <<= 1;
    while (W < (int)f[0].size()) W <<= 1;
    dat.assign(2 * H - 1, vector<i64>(2 * W - 1, BIG));
    init(f);
  }
  void init(vector<vector<i64>> &f) {
    for (int i = 0; i < (int)f.size(); i++)
      for (int j = 0; j < (int)f[0].size(); j++)
        dat[i + H - 1][j + W - 1] = f[i][j];
    for (int i = 2 * H - 2; i > H - 2; i--)
      for (int j = W - 2; j >= 0; j--)
        dat[i][j] = min(dat[i][2 * j + 1], dat[i][2 * j + 2]);
    for (int i = H - 2; i >= 0; i--)
      for (int j = 0; j < 2 * W - 1; j++)
        dat[i][j] = min(dat[2 * i + 1][j], dat[2 * i + 2][j]);
  }
  i64 minimum(int li, int lj, int ri, int rj) {
    return minimum_h(li, lj, ri, rj, 0, H, 0);
  }
  i64 minimum_h(int li, int lj, int ri, int rj, int si, int ti, int k) {
    if (ri <= si or ti <= li) return BIG;
    if (li <= si and ti <= ri) return minimum_w(lj, rj, 0, W, k, 0);
    const int mi = (si + ti) / 2;
    return min(minimum_h(li, lj, ri, rj, si, mi, 2 * k + 1),
               minimum_h(li, lj, ri, rj, mi, ti, 2 * k + 2));
  }
  i64 minimum_w(int lj, int rj, int sj, int tj, int i, int k) {
    if (rj <= sj or tj <= lj) return BIG;
    if (lj <= sj and tj <= rj) return dat[i][k];
    const int mj = (sj + tj) / 2;
    return min(minimum_w(lj, rj, sj, mj, i, 2 * k + 1),
               minimum_w(lj, rj, mj, tj, i, 2 * k + 2));
  }
};

i64 solve() {
  i64 n, m, a, b, g0, x, y, z;
  cin >> n >> m >> a >> b;
  cin >> g0 >> x >> y >> z;
  vector h(n, vector(m, 0LL));
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
  REP(i, n) { DEBUG(h[i]); }
  segtree seg(h);
  i64 ans = 0;
  REP(i, n - a + 1) {
    REP(j, m - b + 1) {
      i64 val = seg.minimum(i, j, i + a, j + b);
      ans += val;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
