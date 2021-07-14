//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
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
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string s;
  cin >> s;
  int N = s.size();
  int S = 0;
  REP(i, N) {
    if (s[i] == 'o') {
      S |= (1 << i);
    }
  }
  S |= (S << N);
  int allon = (1 << N) - 1;
  int minnum = 10;

  auto solve = [&](auto rec, int n, int k, int l, int state) -> void {
    if (n >= minnum) return;
    if ((state & allon) == allon) {
      minnum = n;
      return;
    }
    for (int i = l; i < N; ++i) {
      int nstate = state | ((S >> i) & allon);
      if ((nstate & ~state) == 0) continue;
      rec(rec, n + 1, k, i + 1, nstate);
    }
  };

  solve(solve, 0, 0, 0, 0);
  cout << minnum << endl;
}
