#include <bits/stdc++.h>
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
using Float = double;
const Float PI = std::acos(Float(-1));
const Float PI2 = PI * 2;

struct Point {
  int x, y;
  int bucket;
  Float arg;
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

Float solve() {
  int n;
  cin >> n;
  vector<Point> points(n);
  vector<int> xs, ys;
  xs.reserve(n);
  ys.reserve(n);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    points[i].x = x;
    points[i].y = y;
    xs.push_back(x);
    ys.push_back(y);
  }
  Compress<int> cx(move(xs)), cy(move(ys));
  REP(i, n) {
    int xi = cx.index(points[i].x) * 10 / cx.size();
    int yi = cy.index(points[i].y) * 10 / cy.size();
    int b = xi * cy.size();
    if (xi & 1) {
      b += cy.size() - 1 - yi;
    } else {
      b += yi;
    }
    points[i].bucket = b;
  }
  sort(ALL(points), [&](const Point &p1, const Point &p2) {
    return tie(p1.bucket, p1.x, p1.y) < tie(p2.bucket, p2.x, p2.y);
  });
  vector<int> idx(n);
  REP(i, n) idx[i] = i;

  Float ans = 0;
  REP(i, n) {
    const auto xi = points[i].x, yi = points[i].y;
    REP(j, n) {
      if (j == i) continue;
      auto xj = points[j].x, yj = points[j].y;
      xj -= xi;
      yj -= yi;
      points[j].arg = atan2((Float)yj, (Float)xj);
    }
    sort(ALL(idx), [&](int p, int q) { return points[p].arg < points[q].arg; });
    Float offset = 0;
    int tail_lap = 0;
    int head = 0, tail = 1;
    if (idx[head] == i) ++head;
    if (idx[tail] == i) ++tail;
    while (head < n) {
      while (tail_lap < 2) {
        Float delta = points[idx[tail]].arg + offset - points[idx[head]].arg;
        if (delta >= PI) break;
        chmax(ans, delta);
        ++tail;
        if (tail < n and idx[tail] == i) ++tail;
        if (tail == n) {
          tail = 0;
          if (idx[tail] == i) ++tail;
          ++tail_lap;
          offset += PI2;
        }
      }
      if (tail_lap == 2) break;
      Float delta =
          PI2 - (points[idx[tail]].arg + offset - points[idx[head]].arg);
      chmax(ans, delta);
      ++head;
      if (head < n and idx[head] == i) ++head;
    }
  }
  return ans * 180.0 / PI;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  cout << solve() << "\n";
}
