#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string S;
  cin >> S;
  const int N = S.size();
  int X, Y;
  cin >> X >> Y;
  int i;
  for (i = 0; S[i] == 'F'; i++) {
    X--;
  }
  i64 vmax = 0, hmax = 0;
  vector<int> vmove, hmove;
  bool vertical = false;
  for (; i < N;) {
    int turn_count = 0;
    while (i < N && S[i] == 'T') {
      turn_count++;
      i++;
    }
    vertical ^= (turn_count % 2 == 1);
    int walk = 0;
    while (i < N && S[i] == 'F') {
      walk++;
      i++;
    }
    if (vertical) {
      vmove.push_back(walk);
      vmax += walk;
    } else {
      hmove.push_back(walk);
      hmax += walk;
    }
  }

  auto check = [&](const vector<int>& moves, int target) -> bool {
    int M = moves.size();
    int T = 56010;
    int offset = T / 2;
    vector<bool> dp(T);
    dp[offset + 0] = true;
    REP(i, M) {
      vector<bool> tmp(T);
      swap(dp, tmp);
      REP(j, T) {
        if (tmp[j]) {
          if (j + moves[i] < T) dp[j + moves[i]] = true;
          if (j - moves[i] >= 0) dp[j - moves[i]] = true;
        }
      }
    }
    return dp[offset + target];
  };
  if (check(vmove, Y) && check(hmove, X)) {
    cout << "Yes\n";
  } else {
    cout << "No\n";
  }
}