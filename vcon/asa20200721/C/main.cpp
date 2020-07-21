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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, k;
  cin >> n >> k;
  int pr, ps, pp;
  cin >> pr >> ps >> pp;
  string t;
  cin >> t;

  i64 score = 0;
  REP(i, n) {
    int pt = 0;
    if (t[i] == 'r') {
      pt += pp;
    } else if (t[i] == 's') {
      pt += pr;
    } else {
      pt += ps;
    }
    if (i >= k && t[i - k] == t[i]) {
      pt = 0;
    }
    if (pt == 0) {
      t[i] = '_';
    }
    score += pt;
  }
  cout << score << endl;
}
