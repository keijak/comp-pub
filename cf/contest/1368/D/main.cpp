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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<i64> a(n);
  for (auto& x : a) cin >> x;
  vector<int> v(22);
  int total = 0;
  REP(i, n) {
    REP(j, 22) {
      if (a[i] & (1LL << j)) {
        v[j]++;
        total++;
      }
    }
  }
  i64 ans = 0;
  while (total) {
    i64 x = 0;
    REP(i, 22) {
      if (v[i]) {
        x |= (1 << i);
        v[i]--;
        total--;
      }
    }
    ans += x * x;
  }
  cout << ans << '\n';
}
