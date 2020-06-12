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
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> A(N), B(N);
  for (auto& x : A) cin >> x;
  for (auto& x : B) cin >> x;

  auto solve = [&]() -> i64 {
    priority_queue<pair<i64, int>> pq;
    int ok_cnt = 0;
    REP(i, N) {
      if (B[i] < A[i]) return -1;
      if (B[i] > A[i]) {
        pq.emplace(B[i], i);
      } else {
        ok_cnt++;
      }
    }
    i64 step = 0;
    while (pq.size()) {
      i64 btop = pq.top().first;
      int bix = pq.top().second;
      pq.pop();
      assert(B[bix] == btop);
      assert(B[bix] > A[bix]);
      int l = (bix - 1 + N) % N;
      int r = (bix + 1) % N;
      i64 bnext = max(B[l], B[r]);
      i64 batch;
      if (A[bix] >= bnext) {
        if ((B[bix] - A[bix]) % (B[l] + B[r]) != 0) return -1;
        batch = (B[bix] - A[bix]) / (B[l] + B[r]);
      } else {
        // Ensure btop - batch*(B[l]+B[r]) < bnext.
        batch = (btop - bnext + (B[l] + B[r] - 1)) / (B[l] + B[r]);
      }
      if (batch == 0) return -1;
      step += batch;
      B[bix] -= batch * (B[l] + B[r]);
      if (B[bix] < A[bix]) return -1;
      if (B[bix] > A[bix]) {
        pq.emplace(B[bix], bix);
      } else {
        ok_cnt++;
      }
    }
    assert(ok_cnt == N);
    return step;
  };
  cout << solve() << endl;
}
