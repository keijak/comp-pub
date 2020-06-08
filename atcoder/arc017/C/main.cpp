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
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  i64 X;
  cin >> N >> X;
  vector<i64> weights(N);
  for (auto& x : weights) cin >> x;
  int n1 = N / 2;
  int n2 = N - n1;
  unordered_map<i64, i64> wcnt;
  REP(i, 1LL << n1) {
    i64 s = i, w = 0;
    REP(j, n1) {
      if (!s) break;
      if (s & 1) {
        w += weights[j];
      }
      s >>= 1;
    }
    ++wcnt[w];
  }
  i64 ans = 0;
  REP(i, 1LL << n2) {
    i64 s = i, w = 0;
    vector<int> ix;
    REP(j, n2) {
      if (!s) break;
      if (s & 1) {
        w += weights[n1 + j];
        ix.push_back(n1 + j);
      }
      s >>= 1;
    }
    // if (wcnt[X - w] > 0) {
    //   i64 first = wcnt[X - w];
    //   DEBUG(w, first);
    //   for (auto k : ix) cerr << ' ' << k;
    //   cerr << endl;
    // }
    ans += wcnt[X - w];
  }
  cout << ans << '\n';
}