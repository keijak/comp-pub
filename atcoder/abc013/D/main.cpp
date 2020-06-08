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
  int N, M;
  i64 D;
  cin >> N >> M >> D;
  vector<int> A(M);
  for (auto& x : A) {
    cin >> x;
    x--;
  }
  vector<int> perm(N), base(N), tmp(N);
  REP(i, N) { perm[i] = base[i] = i; }
  REP(i, M) { swap(base[A[i]], base[A[i] + 1]); }

  // Doubling.
  i64 d = D;
  for (int i = 0; d != 0; ++i) {
    if (d & 1) {
      // apply base
      REP(j, N) { tmp[j] = base[perm[j]]; }
      swap(perm, tmp);
    }
    REP(j, N) { tmp[j] = base[base[j]]; }
    swap(base, tmp);
    d >>= 1;
  }

  vector<int> ans(N);
  REP(i, N) { ans[perm[i]] = i; }
  REP(i, N) { cout << (ans[i] + 1) << '\n'; }
}
