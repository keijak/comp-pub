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
const double EPS = 1e-8;
const double PI = cos(-1);

// inner product
// Checks if two vectors are orthogonal.
// double dot(const P& p1, const P& p2) {
//   return real(p1) * real(p2) + imag(p1) * imag(p2);
// }

// outer product
// Checks if two vectors are parallel.
double det(const P& p1, const P& p2) {
  return -real(p1) * imag(p2) + imag(p1) * real(p2);
}

/*
double auc(vector<P>& pu, vector<P>& pl) {
  assert(!pu.empty() && !pl.empty());
  assert(pu.size() == pl.size());
  P *ppu = &pu.front(), *ppl = &pl.front();
  double area = 0.0;
  for (int i = 1; i < (int)pu.size(); ++i) {
    assert(pu[i].real() == pl[i].real());
    double dx = pu[i].real() - ppu->real();
    double dy1 =
        max(pu[i].imag(), ppu->imag()) - max(pl[i].imag(), ppl->imag());
    double dy2 =
        min(pu[i].imag(), ppu->imag()) - min(pl[i].imag(), ppl->imag());
    area += dx * (dy1 + dy2) / 2.0;
    ppu = &pu[i];
    ppl = &pl[i];
  }
  return area;

  // for (auto& p : ps) cout << ' ' << p;
  // cout << endl;
}
*/
// Convex-hull wrapping.
void scan_convex_hull(vector<pair<i64, i64>>& ps, vector<P>& upper,
                      vector<P>& lower) {
  for (int i = 0; i < (int)ps.size(); ++i) {
    double ax = ps[i].first, ay = ps[i].second;
    P now{ax, ay};
    while (upper.size() >= 2) {
      P& p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (det(v1, v2) > EPS) {
        break;
      }
      upper.pop_back();
    }
    upper.push_back(move(now));
  }

  for (int i = ps.size() - 1; i >= 0; --i) {
    double ax = ps[i].first, ay = ps[i].second;
    P now{ax, ay};
    while (lower.size() >= 2) {
      P& p2 = lower[lower.size() - 2];
      P v1 = lower.back() - p2;
      P v2 = now - p2;
      if (det(v1, v2) > EPS) {
        break;
      }
      lower.pop_back();
    }
    lower.push_back(move(now));
  }
  reverse(lower.begin(), lower.end());
}

double area_below(vector<P>& ps) {
  assert(!ps.empty());
  P* pp = &ps.front();
  double area = 0.0;
  for (int i = 1; i < (int)ps.size(); ++i) {
    double dx = ps[i].real() - pp->real();
    double dy1 = max(ps[i].imag(), pp->imag());
    double dy2 = min(ps[i].imag(), pp->imag());
    area += dx * (dy1 + dy2) / 2.0;
    pp = &ps[i];
  }
  return area;
}

int main() {
  int N;
  cin >> N;
  vector<pair<i64, i64>> A(N), B(N);
  REP(i, N) {
    i64 x, y;
    cin >> x >> y;
    A[i] = {x, y};
  }
  REP(i, N) {
    i64 x, y;
    cin >> x >> y;
    B[i] = {x, y};
  }
  sort(A.begin(), A.end());
  sort(B.begin(), B.end());
  vector<P> a_upper, a_lower, b_upper, b_lower;
  a_upper.reserve(N);
  a_lower.reserve(N);
  b_upper.reserve(N);
  b_lower.reserve(N);
  scan_convex_hull(A, a_upper, a_lower);

  // debug("a:");
  // for (auto& p : A) cout << " (" << p.first << ", " << p.second << ")";
  // cout << endl;
  // debug("a_upper:");
  // for (auto& p : a_upper) cout << ' ' << p;
  // cout << endl;
  // debug("a_lower:");
  // for (auto& p : a_lower) cout << ' ' << p;
  // cout << endl;

  scan_convex_hull(B, b_upper, b_lower);

  double area1 = area_below(a_upper) - area_below(a_lower);
  double area2 = area_below(b_upper) - area_below(b_lower);
  if (area1 < EPS) {
    double dmax1 = abs(a_upper.front() - a_upper.back());
    double dmax2 = abs(b_upper.front() - b_upper.back());
    printf("%.12f\n", dmax2 / dmax1);
  } else {
    printf("%.12f\n", sqrt(area2 / area1));
  }
}
