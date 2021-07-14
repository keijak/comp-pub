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
  int n, m;
  cin >> n >> m;
  vector<vector<int>> conn(n);
  REP(i, m) {
    int k;
    cin >> k;
    REP(j, k) {
      int s;
      cin >> s;
      s--;
      conn[s].push_back(i);
    }
  }
  vector<int> p(m);
  REP(i, m) { cin >> p[i]; }

  i64 ans = 0;
  const int L = 1LL << n;
  REP(state, L) {
    vector<int> lamp = p;
    REP(i, n) {
      if (state & (1 << i)) {
        for (int k : conn[i]) {
          lamp[k] ^= 1;
        }
      }
    }
    int num_on = 0;
    REP(i, m) {
      if (lamp[i] == 0) ++num_on;
    }
    if (num_on == m) {
      ++ans;
    }
  }
  cout << ans << endl;
}
