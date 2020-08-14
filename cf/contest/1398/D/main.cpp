#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  V<int> nums(3);
  cin >> nums[0] >> nums[1] >> nums[2];
  V<V<int>> vs(3);
  REP(i, 3) {
    vs[i].resize(nums[i]);
    REP(j, nums[i]) cin >> vs[i][j];
    sort(vs[i].begin(), vs[i].end());
  }

  V<V<V<i64>>> dp(nums[0] + 1, V<V<i64>>(nums[1] + 1, V<i64>(nums[2] + 1, -1)));
  auto solve = [&](auto self, int nr, int ng, int nb) -> i64 {
    if (dp[nr][ng][nb] >= 0) return dp[nr][ng][nb];
    i64 res = 0;
    if (nr > 0 && ng > 0) {
      i64 area = (i64)vs[0][nr - 1] * vs[1][ng - 1];
      i64 a = area + self(self, nr - 1, ng - 1, nb);
      res = max(res, a);
    }
    if (ng > 0 && nb > 0) {
      i64 area = (i64)vs[1][ng - 1] * vs[2][nb - 1];
      i64 a = area + self(self, nr, ng - 1, nb - 1);
      res = max(res, a);
    }
    if (nr > 0 && nb > 0) {
      i64 area = (i64)vs[0][nr - 1] * vs[2][nb - 1];
      i64 a = area + self(self, nr - 1, ng, nb - 1);
      res = max(res, a);
    }
    return dp[nr][ng][nb] = res;
  };

  cout << solve(solve, nums[0], nums[1], nums[2]) << endl;
}
