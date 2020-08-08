#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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

struct Factorials {
  vector<long double> fact;
  Factorials(int n) : fact(n + 1) {
    fact[0] = 0;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] + log(i);
  }
};

using Real = long double;

int main() {
  int N;
  i64 D, X, Y;
  cin >> N >> D >> X >> Y;
  X = abs(X);
  Y = abs(Y);
  V<V<Real>> C(N + 1, V<Real>(N + 1));
  REP(i, N + 1) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }

  auto solve = [&]() -> Real {
    if (X % D != 0 || Y % D != 0) return 0;
    i64 x = X / D, y = Y / D;
    if (x + y > N) return 0;
    if ((N - x - y) % 2 == 1) return 0;

    Real den = pow((Real)4.0, N);
    Real ans = 0;
    for (int q = y; q <= N - x; ++q) {
      int p = N - q;
      if ((q - y) % 2 != 0 || (p - x) < 0 || (p - x) % 2 != 0) continue;
      int d = (q - y) / 2;
      int l = (p - x) / 2;
      ans += C[N][q] * C[q][d] * C[p][l] / den;
    }
    return ans;
  };
  printf("%.12Lf\n", solve());
}
