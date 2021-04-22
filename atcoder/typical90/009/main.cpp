#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <type_traits>
#include <vector>

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

struct Input {
  inline operator int() { return read_int<int>(); }
  inline operator long long() { return read_int<long long>(); }
  inline operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  static T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; isdigit(ch); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;

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
  int n = input;
  vector<Point> points(n);
  vector<int> xs, ys;
  xs.reserve(n);
  ys.reserve(n);
  REP(i, n) {
    int x = input;
    int y = input;
    points[i].x = x;
    points[i].y = y;
    xs.push_back(x);
    ys.push_back(y);
  }
  Compress<int> cx(move(xs)), cy(move(ys));
  REP(i, n) {
    int xi = cx.index(points[i].x) * 10 / cx.size();
    int yi = cy.index(points[i].y) * 10 / cy.size();
    int b = xi * 10 + yi;
    points[i].bucket = b;
  }
  sort(ALL(points), [&](const Point &p1, const Point &p2) {
    return tie(p1.bucket, p1.x, p1.y) < tie(p2.bucket, p2.x, p2.y);
  });

  Float ans = 0;
  vector<Float> args;
  args.reserve(n - 1);
  REP(i, n) {
    const int xi = points[i].x, yi = points[i].y;
    args.clear();
    REP(j, n) {
      if (j == i) continue;
      int xj = points[j].x, yj = points[j].y;
      xj -= xi;
      yj -= yi;
      Float arg = atan2((Float)yj, (Float)xj);
      args.push_back(arg);
    }
    sort(ALL(args));
    const int m = args.size();
    Float offset = 0;
    int tail_lap = 0;
    int head = 0, tail = 1;
    while (head < m) {
      while (true) {
        Float delta = args[tail] + offset - args[head];
        if (delta >= PI) break;
        chmax(ans, delta);
        ++tail;
        if (tail == m) {
          tail = 0;
          ++tail_lap;
          offset += PI * 2;
        }
      }
      Float delta = PI2 - (args[tail] + offset - args[head]);
      chmax(ans, delta);
      ++head;
    }
  }
  return ans * 180.0 / PI;
}

int main() { printf("%.15lf\n", solve()); }
