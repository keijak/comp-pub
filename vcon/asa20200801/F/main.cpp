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
  int n, k;
  cin >> n >> k;
  string s;
  cin >> s;
  vector<int> v(26);
  REP(i, k) { v[s[i] - 'a']++; }
  map<vector<int>, int> mp;
  mp[v] = k - 1;
  for (int i = k; i < n; ++i) {
    v[s[i] - 'a']++;
    int j = i - k;
    v[s[j] - 'a']--;
    auto it = mp.find(v);
    if (it == mp.end()) {
      mp[v] = i;
    } else if (it->second < j + 1) {
      cout << "YES\n";
      return 0;
    }
  }
  cout << "NO\n";
}
