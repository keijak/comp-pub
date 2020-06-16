//#define DEBUGGING  // Enables DEBUG macro.
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
const double PI = cos(-1);

double dot(const P& p1, const P& p2) {
  return real(p1) * real(p2) + imag(p1) * imag(p2);
}

double det(const P& p1, const P& p2) {
  return real(p1) * imag(p2) - imag(p1) * real(p2);
}

pair<P, double> center(const P& p1, const P& p2, const P& p3) {
  if (abs(det(p2 - p1, p3 - p1)) < EPS) {
    double d1 = abs(p2 - p3);
    double d2 = abs(p1 - p3);
    double d3 = abs(p2 - p1);
    if (d1 >= max(d2, d3) - EPS) {
      return {(p2 + p3) / 2.0, d1 / 2.0};
    }
    if (d2 >= max(d1, d3) - EPS) {
      return {(p1 + p3) / 2.0, d2 / 2.0};
    }
    if (d3 >= max(d2, d1) - EPS) {
      return {(p2 + p1) / 2.0, d3 / 2.0};
    }
    assert(false);
  }

  P r1 = (p1 + p2) / 2.0;
  P q1 = (p2 - p1) * P(0.0, 1.0);
  q1 /= abs(q1);

  P r2 = (p2 + p3) / 2.0;
  P q2 = (p3 - p2) * P(0.0, 1.0);
  q2 /= abs(q2);

  double de1 = det(q2, q1);
  assert(abs(de1) > EPS);
  double de2 = det(q2, r2 - r1);
  double t = de2 / de1;
  P center = r1 + t * q1;
  double radius =
      (abs(p1 - center) + abs(p2 - center) + abs(p3 - center)) / 3.0;
  assert(abs(radius - abs(p1 - center)) < EPS);
  assert(abs(radius - abs(p2 - center)) < EPS);
  assert(abs(radius - abs(p3 - center)) < EPS);

  P r3 = (p3 + p1) / 2.0;
  P q3 = (p1 - p3) * P(0.0, 1.0);
  q3 /= abs(q3);
  assert(dot(p1 - p3, center - r3) < EPS);
  assert(abs(det(center - r3, q3)) < EPS);

  return {center, radius};
}

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

  double minr = 1e+9;
  REP(i, N) REP(j, i) REP(k, j) {
    const P& p1 = points[i];
    const P& p2 = points[j];
    const P& p3 = points[k];
    auto centerrad = center(p1, p2, p3);
    const P& c = centerrad.first;
    double r = centerrad.second;
    if (r < minr && contains_all(c, r)) {
      minr = r;
    }
  }
  printf("%.18f\n", minr);
}
