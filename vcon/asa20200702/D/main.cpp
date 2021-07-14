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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<tuple<i64, i64, i64>> cones(N);
  REP(i, N) {
    i64 x, r, h;
    cin >> x >> r >> h;
    cones[i] = {x, r, h};
  }
  REP(q, Q) {
    i64 a, b;
    cin >> a >> b;
    double total = 0.0;
    REP(i, N) {
      auto [x, r, h] = cones[i];
      if (b <= x) continue;
      if (a >= x + h) continue;
      double ha = min<double>(h, x + h - a);
      double ra = r * ha / h;
      double v1 = M_PI * ra * ra * ha / 3.0;
      double hb = max<double>(x + h - b, 0);
      double rb = r * hb / h;
      double v2 = M_PI * rb * rb * hb / 3.0;
      assert(v1 >= v2);
      total += v1 - v2;
    }
    cout << fixed << total << '\n';
  }
}
