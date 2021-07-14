#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

using P = complex<double>;
const double EPS = 1e-6;
const double PI = cos(-1);

// inner product
// Checks if two vectors are orthogonal.
double dot(const P& p1, const P& p2) {
  return real(p1) * real(p2) + imag(p1) * imag(p2);
}

// outer product
// Checks if two vectors are parallel.
double det(const P& p1, const P& p2) {
  return imag(p1) * real(p2) - real(p1) * imag(p2);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  double x0, y0;
  cin >> x0 >> y0;
  P pr = {x0, y0};
  int N;
  cin >> N;
  vector<P> ps(N);
  REP(i, N) {
    double x, y;
    cin >> x >> y;
    ps[i] = {x, y};
  }
  double ans = 10000000000.0;
  for (int i = 0; i < N; ++i) {
    int k = (i - 1 + N) % N;
    P q = ps[i] - ps[k];
    double t = dot(q, pr - ps[k]) / dot(q, q);
    P s = ps[k] + t * q;
    double d = abs(s - pr);
    ans = min(ans, d);
  }
  cout << fixed << setprecision(10) << ans << endl;
}
