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

void solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  REP(i, n) cin >> a[i];
  string s(a[0] + 1, 'a');
  cout << s << '\n';
  REP(i, n) {
    string t;
    int cnt = 0;
    REP(j, a[i]) { t.push_back(s[j]); }
    char ch = 'a';
    if (a[i] < s.size()) {
      ch = ((s[a[i]] - 'a' + 1) % 26) + 'a';
    }
    t.push_back(ch);
    if (i < n - 1) {
      while (t.size() < a[i + 1]) {
        t.push_back(ch);
      }
    }
    cout << t << '\n';
    swap(s, t);
  }
  //
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}
