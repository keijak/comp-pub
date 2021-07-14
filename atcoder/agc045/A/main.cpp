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

int solve() {
  int N;
  cin >> N;
  vector<u64> A(N);
  for (auto& x : A) cin >> x;
  cin.ignore();
  string S;
  getline(cin, S);
  assert((int)S.size() == N);
  int p = N - 1;
  vector<bitset<64>> vecs(64);
  auto can_make = [&](u64 x) -> bool {
    bitset<64> b(x);
    REP(i, 64) {
      if (b[i]) {
        b ^= vecs[i];
      }
    }
    return b.to_ullong() == 0LL;
  };
  auto maybe_add = [&](u64 x) -> void {
    if (can_make(x)) return;
    bitset<64> b(x);
    REP(i, 64) {
      if (b[i]) {
        b ^= vecs[i];
      }
    }
    int first = -1;
    REP(i, 64) {
      if (b[i]) {
        first = i;
        break;
      }
    }
    assert(first >= 0);
    vecs[first] = b;
    REP(i, 64) {
      if (i != first && vecs[i][first]) {
        vecs[i] ^= b;
      }
    }
  };
  while (p >= 0) {
    while (p >= 0 && S[p] == '0') {
      maybe_add(A[p]);
      --p;
    }
    while (p >= 0 && S[p] == '1') {
      if (!can_make(A[p])) return 1;
      --p;
    }
  }
  return 0;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  REP(t, T) { cout << solve() << '\n'; }
}
