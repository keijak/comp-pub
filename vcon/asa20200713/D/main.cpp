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
  int N;
  cin >> N;
  string S;
  cin >> S;
  bitset<10> d1;
  bitset<100> d2;
  set<int> ans;
  REP(i, N) {
    int d = S[i] - '0';
    REP(j, 100) {
      if (d2[j]) {
        ans.insert(j * 10 + d);
      }
    }
    REP(j, 10) {
      if (d1[j]) {
        d2.set(10 * j + d, true);
      }
    }
    d1.set(d, true);
  }
  cout << ans.size() << endl;
}
