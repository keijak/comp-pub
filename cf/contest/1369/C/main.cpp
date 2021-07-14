#define MY_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if MY_DEBUG
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
#else
#define debug(...)
#define DEBUG(...)
#endif

i64 solve() {
  int n, k;
  cin >> n >> k;
  vector<i64> a(n);
  for (auto& x : a) cin >> x;
  vector<int> w(k);
  for (auto& x : w) cin >> x;
  sort(a.begin(), a.end());
  sort(w.begin(), w.end());
  int ones = 0;
  REP(i, k) {
    if (w[i] == 1)
      ++ones;
    else
      break;
  }
  i64 ans = 0;
  REP(i, ones) { ans += 2 * a[n - 1 - i]; }
  int m = n - ones - 1;
  int s = 0;
  for (int i = k - 1; i >= ones; --i) {
    ans += a[m];  // max
    ans += a[s];  // min
    s += w[i] - 1;
    m--;
  }
  // DEBUG(ans, s, m);
  // assert(s == m);

  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    cout << solve() << '\n';
  }
}
