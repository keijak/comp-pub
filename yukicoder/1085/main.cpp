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

const i64 M = 1'000'000'007LL;
const int K = 1'000'000;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string T;
  getline(cin, T);
  int D;
  cin >> D;

  set<int> cands;
  vector<int> dsum(K);
  REP(i, 10) dsum[i] = i;
  for (int i = 10; i < K; ++i) {
    int ds = 0;
    int x = i;
    while (x) {
      ds += x % 10;
      x /= 10;
    }
    dsum[i] = dsum[ds];
    if (dsum[i] == D) {
      cands.insert(i);
    }
  }

  int unk = 0;
  int ds = 0;
  REP(i, T.size()) {
    if (T[i] == '?') {
      unk++;
    } else {
      int d = T[i] - '0';
      ds += d;
    }
  }
  DEBUG(ds, unk);
  auto it = cands.lower_bound(ds);
  while (it != cands.end()) {
    cout << *it << endl;
    ++it;
  }
}
