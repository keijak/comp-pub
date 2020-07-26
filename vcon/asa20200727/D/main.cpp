#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  priority_queue<tuple<i64, i64, int>> ha;
  priority_queue<tuple<i64, i64, int>> hb;
  REP(i, N) {
    i64 a, b;
    cin >> a >> b;
    ha.emplace(a + b, a, i);
    hb.emplace(b + a, b, i);
  }
  i64 sa = 0, sb = 0;
  vector<bool> taken(N, false);
  REP(i, N) {
    if (i % 2 == 0) {
      i64 h, a, j;
      for (;;) {
        tie(h, a, j) = ha.top();
        if (!taken[j]) break;
        ha.pop();
      }
      taken[j] = true;
      sa += a;
    } else {
      i64 h, b, j;
      for (;;) {
        tie(h, b, j) = hb.top();
        if (!taken[j]) break;
        hb.pop();
      }
      taken[j] = true;
      sb += b;
    }
  }
  cout << (sa - sb) << endl;
}
