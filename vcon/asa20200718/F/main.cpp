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

const int L = 201000;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int K;
  cin >> K;
  string S;
  cin >> S;
  vector<set<int>> ax(2 * L), ay(2 * L);
  int curx = 0, cury = 0;
  ax[L].insert(1);
  ax[L].insert(-1);
  ay[L].insert(1);
  ay[L].insert(-1);
  set<pair<int, int>> visited;
  visited.emplace(0, 0);
  REP(i, K) {
    if (S[i] == 'R') {
      auto it = ax[L + cury].upper_bound(curx);
      assert(it != ax[L + cury].end());
      curx = *it;
    } else if (S[i] == 'L') {
      auto it = ax[L + cury].lower_bound(curx);
      assert(it != ax[L + cury].begin());
      --it;
      curx = *it;
    } else if (S[i] == 'U') {
      auto it = ay[L + curx].upper_bound(cury);
      assert(it != ay[L + curx].end());
      cury = *it;
    } else {
      assert(S[i] == 'D');
      auto it = ay[L + curx].lower_bound(cury);
      assert(it != ay[L + curx].begin());
      --it;
      cury = *it;
    }
    DEBUG(curx, cury);
    visited.emplace(curx, cury);
    ax[L + cury].erase(curx);
    ay[L + curx].erase(cury);
    if (!visited.count({curx + 1, cury})) {
      ax[L + cury].insert(curx + 1);
    }
    if (!visited.count({curx - 1, cury})) {
      ax[L + cury].insert(curx - 1);
    }
    if (!visited.count({curx, cury + 1})) {
      ay[L + curx].insert(cury + 1);
    }
    if (!visited.count({curx, cury - 1})) {
      ay[L + curx].insert(cury - 1);
    }
  }
  cout << curx << " " << cury << endl;
}
