#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

using P = complex<double>;
const double EPS = 1e-6;

int main() {
  int N;
  cin >> N;
  vector<P> points;
  REP(i, N) {
    int x, y;
    cin >> x >> y;
    points.emplace_back(double(x), double(y));
  }

  if (N == 2) {
    double ans = abs(points[0] - points[1]) / 2.0;
    printf("%.18f\n", ans);
    return 0;
  }
  assert(N >= 3);

  auto contains_all = [&](const P& center, double radius) -> bool {
    REP(i, N) {
      double d = abs(points[i] - center);
      if (d > radius + EPS) {
        return false;
      }
    }
    return true;
  };

  auto check = [&](double r) -> bool {
    REP(i, N) REP(j, i) {
      const P& p1 = points[i];
      const P& p2 = points[j];
      if (abs(p2 - p1) > 2.0 * r + EPS) return false;

      P m = (p1 + p2) / 2.0;
      P x = (p2 - p1) * P(0.0, 1.0);
      x /= abs(x);
      double b = abs(m - p1);
      double h = sqrt(r * r - b * b);
      x *= h;
      vector<P> ps = {m + x, m - x};
      for (auto& p : ps) {
        if (contains_all(p, r)) return true;
      }
    }
    return false;
  };

  double ng = 0.0, ok = 1e6;
  REP(_, 50) {
    double r = (ok + ng) / 2.0;
    if (check(r)) {
      ok = r;
    } else {
      ng = r;
    }
  }

  printf("%.18f\n", ok);
}
