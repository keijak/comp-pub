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
  int N;
  cin >> N;
  vector<i64> A(N);
  vector<i64> cnt(100001);
  i64 sm = 0;
  for (auto& x : A) {
    cin >> x;
    cnt[x]++;
    sm += x;
  }

  int Q;
  cin >> Q;
  REP(i, Q) {
    i64 b, c;
    cin >> b >> c;
    sm += (c - b) * cnt[b];
    cout << sm << '\n';
    cnt[c] += cnt[b];
    cnt[b] = 0;
  }
}
