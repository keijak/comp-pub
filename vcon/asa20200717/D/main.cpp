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
  int N;
  cin >> N;
  vector<i64> A(N);
  map<i64, int> ma;
  for (auto& x : A) {
    cin >> x;
    ma[x]++;
  }
  vector<pair<i64, int>> va;
  int over = 0;
  for (auto [x, k] : ma) {
    va.emplace_back(x, k);
    over += k - 1;
  }
  int uniq = va.size();
  int ans = (over % 2) ? (uniq - 1) : uniq;
  cout << ans << endl;
}
