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

bool solve(int N) {
  vector<int> A(N), B(N);
  REP(i, N) { cin >> A[i]; }
  int cnt[2] = {0, 0};
  for (auto& x : B) {
    cin >> x;
    cnt[x]++;
  }

  bool is_sorted = true;
  for (int i = 1; i < N; ++i) {
    if (A[i - 1] > A[i]) is_sorted = false;
  }
  if (is_sorted) return true;

  if (cnt[0] == 0 || cnt[1] == 0) {
    return false;
  }

  //   REP(i, N) { cnt[A[i]][B[i]]++; }
  //   sort(S.begin(), S.end());

  //   int cur = 0;
  //   while (cur < N) {
  //     int start = cur;
  //     int v = S[cur].first;
  //     int need[2] = {0, 0};
  //     while (cur < N && S[cur] == v) {
  //       need[B[S[cur].second]]++;
  //       cur++;
  //     }
  //     int end = cur;

  //     for (int i = start; i < end; ++i) {
  //       if (A[i] != v) {
  //         assert(A[i] > v);
  //       }
  //     }
  //   }

  //   int head = 0, tail = N - 1;
  //   while (head < tail) {
  //     if (head == S[head].second) {
  //       head++;
  //       continue;
  //     }
  //     if (tail == S[tail].second) {
  //       tail--;
  //       continue;
  //     }

  //     if (cnt[B[A[head].second]] > cnt[B[A[tail].second]]) {
  //       int color = B[A[head].second];
  //       if (cnt[1 - color] == 0) return false;
  //       cnt[color]--;
  //       head++;
  //     } else {
  //       int color = B[A[tail].second];
  //       if (cnt[1 - color] == 0) return false;
  //       cnt[color]--;
  //       tail--;
  //     }
  //   }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    cout << (solve(n) ? "Yes\n" : "No\n");
  }
}