//#define DEBUGGING  // Enables DEBUG macro.
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

i64 solve() {
  int N, K;
  cin >> N >> K;
  cin.ignore();
  string S;
  getline(cin, S);
  assert(S.size() == (size_t)N);

  vector<int> ones;
  ones.reserve(N);
  REP(i, N) {
    if (S[i] == '1') {
      ones.push_back(i);
    }
  }
  int ans = 0;
  for (int i = 1; i < (int)ones.size(); ++i) {
    int x1 = ones[i - 1];
    int x2 = ones[i];
    ans += (x2 - x1 - 1 - K) / (K + 1);
    DEBUG(0, i, ans);
  }
  if (ones.size() > 0) {
    ans += ones[0] / (K + 1);
    DEBUG(1, ans);
    ans += (N - ones.back() - 1) / (K + 1);
    DEBUG(2, ans);
  } else {
    ans += (N + K) / (K + 1);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    cout << solve() << '\n';
  }
}