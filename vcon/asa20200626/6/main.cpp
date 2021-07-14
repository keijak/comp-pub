#define MY_DEBUG 0
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if MY_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
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
  int Q;
  cin >> Q;
  REP(i, Q) {
    i64 k;
    cin >> k;

    i64 nd = 0, nm = 0, ndm = 0, ans = 0;
    for (int i = 0; i < N; ++i) {
      if (i >= k) {
        int j = i - k;
        if (S[j] == 'D') {
          nd--;
          ndm -= nm;
        } else if (S[j] == 'M') {
          nm--;
        }
      }

      if (S[i] == 'D') {
        nd++;
      } else if (S[i] == 'M') {
        nm++;
        ndm += nd;
      } else if (S[i] == 'C') {
        ans += ndm;
      }
    }
    cout << ans << '\n';
  }
}
