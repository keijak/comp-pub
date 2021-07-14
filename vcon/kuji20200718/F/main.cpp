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

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto& x : p) cin >> x;

  map<int, int> lmax, rmax;
  i64 ans = 0;
  vector<int> lless(n), rless(n);
  REP(i, n) {
    auto it = lmax.lower_bound(p[i]);
    int lstreak = 0;
    if (it != lmax.end()) {
      lstreak = i - it->second;
    }
    lless[i] = lstreak;
    lmax[p[i]] = i;
  }
  for (int i = n - 1; i >= 0; --i) {
    auto it = rmax.lower_bound(p[i]);
    int rstreak = 0;
    if (it != rmax.end()) {
      rstreak = it->second - i;
    }
    rless[i] = rstreak;
    rmax[p[i]] = i;
  }
  i64 ans = 0;
  REP(i, n) { ans += p[i] * (lless[i] + rless[i] + 2); }
}
