#define DEBUGGING  // Enables DEBUG macro.
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

  int T;
  cin >> T;
  while (T--) {
    string B;
    cin >> B;
    string A;
    A.push_back(B[0]);
    for (int i = 1; i < B.size(); i += 2) {
      A.push_back(B[i]);
    }
    cout << A << '\n';
  }
}
