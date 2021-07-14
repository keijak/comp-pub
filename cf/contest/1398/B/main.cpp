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
  string s;
  cin >> s;
  V<int> v;
  v.reserve(s.size());
  int i = 0;
  int n = s.size();
  for (; i < n;) {
    while (i < n && s[i] == '0') ++i;
    if (i == n) break;
    int cnt = 0;
    while (i < n && s[i] == '1') {
      ++i;
      ++cnt;
    }
    v.push_back(cnt);
  }
  sort(v.rbegin(), v.rend());
  i64 ans = 0;
  for (int i = 0; i < v.size(); i += 2) {
    ans += v[i];
  }

  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) cout << solve() << '\n';
}
