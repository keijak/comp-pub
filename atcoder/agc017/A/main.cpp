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
  int N, P;
  cin >> N >> P;
  vector<int> A(N);
  for (auto& x : A) cin >> x;
  u64 ne = 1, no = 0;
  REP(i, N) {
    if (A[i] % 2 == 0) {
      ne = ne * 2;
      no = no * 2;
    } else {
      ne = no = no + ne;
    }
  }
  if (P) {
    cout << no << endl;
  } else {
    cout << ne << endl;
  }
}