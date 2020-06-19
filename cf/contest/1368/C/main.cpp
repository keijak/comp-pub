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

int dr[] = {0, 0, 1, -1};
int dc[] = {1, -1, 0, 0};

void draw1(int n) {
  cout << (3 * n + 4) << '\n';
  cout << 0 << ' ' << 0 << '\n';
  REP(i, n + 1) {
    cout << (i + 1) << ' ' << i << '\n';
    cout << (i) << ' ' << (i + 1) << '\n';
    cout << (i + 1) << ' ' << (i + 1) << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  draw1(N);
}
