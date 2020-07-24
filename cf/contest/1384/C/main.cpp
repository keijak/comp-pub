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

i64 solve() {
  int n;
  cin >> n;
  string sa, sb;
  cin >> sa >> sb;
  assert(sa.size() == size_t(n));
  assert(sb.size() == size_t(n));

  vector<set<char>> moveto(26);  // indices
  REP(i, n) {
    if (sa[i] > sb[i]) return -1;
    if (sa[i] == sb[i]) continue;
    moveto[sa[i] - 'a'].insert(sb[i]);
  }
  int cnt = 0;
  REP(i, 26) {
    if (moveto[i].empty()) continue;
    char y = *moveto[i].begin();
    moveto[i].erase(y);
    for (char z : moveto[i]) {
      moveto[y - 'a'].insert(z);
    }
    ++cnt;
  }

  return cnt;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << solve() << '\n';
  }
}
