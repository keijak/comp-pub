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
  string s;
  cin >> s;
  int a = s[0] - '0';
  int b = s[1] - '0';
  int c = s[2] - '0';
  int d = s[3] - '0';

  REP(i, 2) {
    REP(j, 2) {
      REP(k, 2) {
        int t = a;
        t += (i == 0 ? 1 : -1) * b;
        char op1 = (i == 0 ? '+' : '-');
        t += (j == 0 ? 1 : -1) * c;
        char op2 = (j == 0 ? '+' : '-');
        t += (k == 0 ? 1 : -1) * d;
        char op3 = (k == 0 ? '+' : '-');
        if (t == 7) {
          printf("%d%c%d%c%d%c%d=7\n", a, op1, b, op2, c, op3, d);
          return 0;
        }
      }
    }
  }
}