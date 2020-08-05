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
  int N;
  scanf("%d", &N);
  vector<int> cum(48 * 60);
  REP(i, N) {
    int h1, m1, h2, m2;
    scanf("%d:%d %d:%d", &h1, &m1, &h2, &m2);
    int t1 = h1 * 60 + m1;
    int t2 = h2 * 60 + m2;
    cum[t1]++;
    cum[t2]--;
    if (t1 <= 12 * 60) {
      cum[t1 + 24 * 60]++;
      cum[t2 + 24 * 60]--;
    }
  }

  int ans = 0;
  REP(i, 36 * 60 + 1) {
    cum[i + 1] += cum[i];
    // for (int i = minstart; i < maxend; ++i) {
    ans = max(ans, cum[i]);
  }
  printf("%d\n", ans);
}
