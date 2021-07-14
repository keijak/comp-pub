#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SORT(v) sort((v).begin(), (v).end())

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
  int N;
  cin >> N;
  V<int> a(N), sa(N);
  unordered_set<int> oddm;
  REP(i, N) {
    int x;
    cin >> x;
    a[i] = sa[i] = x;
  }
  SORT(sa);
  REP(i, N) {
    if (i % 2 == 1) oddm.insert(sa[i]);
  }
  int ans = 0;
  REP(i, N) {
    if (i % 2 == 0 && oddm.count(a[i])) ++ans;
  }
  cout << ans << endl;
}
