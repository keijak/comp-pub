#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

i64 solve() {
  int n;
  cin >> n;
  V<int> a(n);
  REP(i, n) cin >> a[i];
  V<int> counter(n + 1);
  int maxc = 0;
  REP(i, n) {
    int x = ++counter[a[i]];
    maxc = max(maxc, x);
  }
  assert(maxc > 1);
  sort(counter.rbegin(), counter.rend());
  V<int> v(maxc);
  int j = 0;
  while (j < counter.size() && counter[j] == maxc) {
    REP(i, maxc) { v[i]++; }
    ++j;
  }
  int k = 0;
  while (j < counter.size()) {
    while (j < counter.size() && counter[j] == 0) ++j;
    if (j == counter.size()) break;
    counter[j]--;
    v[k]++;
    k = (k + 1) % (maxc - 1);
  }
  REP(i, maxc) { DEBUG(i, v[i]); }
  int ans = v[maxc - 2] - 1;
  return ans;
}
//   int k = 0;
//   REP(i, n) {
//     if (counter[i] == maxc) {
//       ++k;
//     }
//   }
//   DEBUG(maxc, k);
//   return (n - k) / (maxc - 1) - 1;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    DEBUG(t);
    cout << solve() << '\n';
  }
}
