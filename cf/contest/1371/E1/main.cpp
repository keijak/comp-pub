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
  int n, p;
  cin >> n >> p;
  DEBUG(n, p);
  vector<int> A(n);
  int amax = 0, amin = 2001;
  for (auto& x : A) {
    cin >> x;
    amax = max(amax, x);
    amin = min(amin, x);
  }
  sort(A.begin(), A.end());

  vector<int> ans;
  for (int x = amax - 1; x >= amin; x--) {
    DEBUG(x);
    int leq = upper_bound(A.begin(), A.end(), x) - A.begin();
    if (leq >= p) {
      DEBUG(x, leq, leq >= p);
      continue;
    }

    bool good = true;
    for (int i = 0, j = n - 1; j >= 0; --j, ++i) {
      if (A[j] <= x) break;
      if (x + (n - 1 - i) < A[j]) {
        DEBUG(x, j, A[j], x + (n - 1 - i));
        good = false;
        break;
      }
      i64 d = n - (A[j] - x) - i;
      if (d % p == 0) {
        DEBUG(x, j, A[j], d);
        good = false;
        break;
      }
    }
    if (good) {
      ans.push_back(x);
    }
  }
  sort(ans.begin(), ans.end());
  cout << ans.size() << '\n';
  REP(i, ans.size()) { cout << (i == 0 ? "" : " ") << ans[i]; }
  cout << endl;
}
