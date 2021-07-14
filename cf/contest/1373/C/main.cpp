#define MY_DEBUG 0
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
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

i64 solve() {
  string s;
  cin >> s;
  int mind = 0;
  int d = 0;
  vector<int> dindex(s.size() + 2, -1);
  REP(i, s.size()) {
    if (s[i] == '-') {
      d--;
    } else {
      d++;
    }
    mind = min(mind, d);
    if (mind < 0 && dindex[-mind] == -1) {
      dindex[-mind] = i;
    }
  }
  REP(i, dindex.size()) { DEBUG(i, dindex[i]); }
  i64 ans = s.size();
  for (int i = 1; i <= -mind; ++i) {
    ans += dindex[i] + 1;
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    DEBUG(T);
    cout << solve() << '\n';
  }
}
