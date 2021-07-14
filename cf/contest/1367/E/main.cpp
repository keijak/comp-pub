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

i64 solve(int test_case) {
  int N, K;
  cin >> N >> K;
  cin.ignore();
  string S;
  getline(cin, S);
  assert(S.size() == (size_t)N);
  sort(S.begin(), S.end());
  vector<i64> counts;
  counts.reserve(N);
  int count = 1;
  for (int i = 1; i < N; ++i) {
    if (S[i] == S[i - 1]) {
      ++count;
    } else {
      counts.push_back(count);
      count = 1;
    }
  }
  counts.push_back(count);
  sort(counts.rbegin(), counts.rend());

  for (auto& c : counts) cerr << ' ' << c;
  cerr << endl;

  i64 ans = 1;
  // Make the ring from top i items.
  REP(i, counts.size()) {
    if (K % (i + 1) == 0) {
      //   i64 c = 0;
      //   REP(j, i + 1) { c += counts[j] / (i + 1); }
      DEBUG(test_case, i, (i + 1) * counts[i]);
      ans = max(ans, (i + 1) * counts[i]);
    }
    DEBUG(test_case, i, ans);
  }

  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  REP(t, T) { cout << solve(t) << '\n'; }
}
