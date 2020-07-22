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
  int L;
  cin >> L;
  vector<unsigned> B(L);
  for (auto& x : B) cin >> x;

  unsigned a = 0;
  REP(i, L) { a ^= B[i]; }
  if (a != 0) {
    cout << -1 << endl;
    return 0;
  }
  cout << 0 << '\n';
  unsigned p = 0;
  for (int i = 0; i < L - 1; ++i) {
    unsigned x = B[i] ^ p;
    cout << x << '\n';
    p = x;
  }
}
