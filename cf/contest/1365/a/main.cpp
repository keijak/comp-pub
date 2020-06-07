#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
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

bool solve(const int N, const int M) {
  vector<bool> rows(N, false), cols(M, false);
  REP(i, N) {
    REP(j, M) {
      int a;
      cin >> a;
      if (a == 1) {
        rows[i] = true;
        cols[j] = true;
      }
    }
  }
  int r = 0, c = 0;
  REP(i, N) if (!rows[i]) r++;
  REP(i, M) if (!cols[i]) c++;
  return (min(r, c) % 2) == 1;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    cout << (solve(n, m) ? "Ashish" : "Vivek") << '\n';
  }
}
