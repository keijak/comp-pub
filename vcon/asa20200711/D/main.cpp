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
  i64 N, K;
  cin >> N >> K;
  string S;
  cin >> S;
  // Reverse it.
  if (S[0] == 'L') {
    REP(i, N) { S[i] = (S[i] == 'L') ? 'R' : 'L'; }
  }

  i64 unhappy = 0;
  for (int i = 0; i < N - 1; ++i) {
    if (S[i] == 'R' && S[i + 1] == 'L') {
      if (K > 0) {
        --K;
        for (int j = i + 1; j < N; ++j) {
          if (S[j] == 'R') {
            i = j - 1;
            break;
          }
          S[j] = 'R';
        }
      } else {
        unhappy += 2;
      }
    }
  }
  if (S[N - 1] == 'R') ++unhappy;
  cout << (N - unhappy) << endl;
}
