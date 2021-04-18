#include <bits/stdc++.h>

#include <atcoder/fenwicktree>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
const i64 VMAX = 1e15;

template <typename T>
struct dynamic_segment {
  struct node;
  using np = node *;
  struct node {
    np ch[2] = {0, 0};
    T val = 0;
  };
  np root = new node();
  np add(int64_t l, int64_t r, int64_t x, T val, np t) {
    if (!(l <= x && x < r)) return t;
    if (!t) t = new node();
    t->val += val;
    if (r - l == 1) return t;
    int64_t m = (l + r) / 2;
    t->ch[0] = add(l, m, x, val, t->ch[0]);
    t->ch[1] = add(m, r, x, val, t->ch[1]);
    return t;
  }
  T get(int64_t l, int64_t r, int64_t a, int64_t b, np t) {
    if (!t) return 0;
    if (b <= l || r <= a) return 0;
    if (a <= l && r <= b) return t->val;
    int64_t m = (l + r) / 2;
    return get(l, m, a, b, t->ch[0]) + get(m, r, a, b, t->ch[1]);
  }
  void add(int64_t x, T val) { root = add(0, 1LL << 32, x, val, root); }
  T get(int64_t l, int64_t r) { return get(0, 1LL << 32, l, r, root); }
};

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

const int YMAX = int(1e8) + 5;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, m, q;
  cin >> n >> m >> q;
  vector<array<int, 3>> queries(q);
  vector<int> ys;
  ys.push_back(0);
  ys.push_back(YMAX);
  REP(i, q) {
    int t, x, y;
    cin >> t >> x >> y;
    --x;
    queries[i] = {t, x, y};
    ys.push_back(y);
  }
  Compress<int> ycomp(ys);
  const int ny = ycomp.size();

  vector<int> a(n), b(m);
  atcoder::fenwick_tree<int> ac(ny), bc(ny);
  atcoder::fenwick_tree<i64> as(ny), bs(ny);
  ac.add(ycomp.index(0), n);
  bc.add(ycomp.index(0), m);

  i64 ans1 = 0, ans2 = 0;
  REP(i, q) {
    auto [t, x, y] = queries[i];
    if (t == 1 and a[x] != y) {
      i64 p = a[x];
      int pi = ycomp.index(p);
      int yi = ycomp.index(y);
      i64 pl = bc.sum(0, pi);
      i64 yl = bc.sum(0, yi);
      ans1 -= pl * a[x];
      ans1 += yl * y;
      if (p < y) {
        ans1 -= bs.sum(pi + 1, yi + 1);
      } else {
        ans1 += bs.sum(yi + 1, pi + 1);
      }
      ans2 -= bs.sum(pi, pi + 1);
      ans2 += bs.sum(yi, yi + 1);
      ac.add(pi, -1);
      ac.add(yi, +1);
      as.add(pi, -p);
      as.add(yi, y);
      a[x] = y;
    } else if (t == 2 and b[x] != y) {
      i64 p = b[x];
      int pi = ycomp.index(p);
      int yi = ycomp.index(y);
      i64 pl = ac.sum(0, pi);
      i64 yl = ac.sum(0, yi);
      ans1 -= pl * p;
      ans1 += yl * y;
      if (p < y) {
        ans1 -= as.sum(pi + 1, yi + 1);
      } else {
        ans1 += as.sum(yi + 1, pi + 1);
      }
      ans2 -= as.sum(pi, pi + 1);
      ans2 += as.sum(yi, yi + 1);
      bc.add(pi, -1);
      bc.add(yi, +1);
      bs.add(pi, -p);
      bs.add(yi, y);
      b[x] = y;
    }

    i64 ans = ans1 + ans2;
    cout << ans << "\n";
  }
}
