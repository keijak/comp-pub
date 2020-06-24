#define ENABLE_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int N;
    cin >> N;
    string S;
    cin >> S;
    assert(S.size() == N);

    int l = -1, r = -1;
    REP(i, N) {
      if (S[i] == '1') {
        l = i;
        break;
      }
    }
    REP(i, N) {
      if (S[N - 1 - i] == '0') {
        r = N - 1 - i;
        break;
      }
    }
    if (l == -1 || r == -1 || r < l) {
      cout << S << '\n';
      continue;
    }
    REP(j, l + 1) { cout << '0'; }
    int m = N - 1 - r;
    REP(j, m) { cout << '1'; }
    cout << '\n';
  }
}
