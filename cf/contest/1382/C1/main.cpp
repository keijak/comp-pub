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
  string sa, sb;
  cin >> sa >> sb;
  deque<int> ra;
  REP(i, n) { ra.push_back(sa[i] - '0'); }

  bool flip = false;
  vector<int> ans;
  for (int p = n - 1; p >= 0; --p) {
    int target = sb[p] - '0';
    if (!flip) {
      if (ra.back() == target) {
        ra.pop_back();
        continue;
      }
      flip = !flip;
      if (ra.front() == target) {
        ans.push_back(1);
      }
      ans.push_back(p + 1);
      ra.pop_front();
    } else {
      if ((ra.front() ^ 1) == target) {
        ra.pop_front();
        continue;
      }
      flip = !flip;
      if ((ra.back() ^ 1) == target) {
        ans.push_back(1);
      }
      ans.push_back(p + 1);
      ra.pop_back();
    }
  }
  cout << ans.size();
  REP(i, ans.size()) { cout << " " << ans[i]; }
  cout << '\n';
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
