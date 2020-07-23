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

void solve() {
  string ss;
  getline(cin, ss);
  int r = 0, s = 0, p = 0;
  for (char ch : ss) {
    switch (ch) {
      case 'R':
        ++r;
        break;
      case 'S':
        ++s;
        break;
      case 'P':
        ++p;
        break;
    }
  }
  if (r >= s && r >= p) {
    REP(i, ss.size()) { cout << 'P'; }
    cout << "\n";
    return;
  }
  if (s >= r && s >= p) {
    REP(i, ss.size()) { cout << 'R'; }
    cout << "\n";
    return;
  }
  if (p >= s && p >= r) {
    REP(i, ss.size()) { cout << 'S'; }
    cout << "\n";
    return;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  cin.ignore();
  while (t--) {
    solve();
  }
}