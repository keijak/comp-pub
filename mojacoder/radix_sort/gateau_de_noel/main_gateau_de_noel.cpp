#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using Int = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}

template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  while (std::abs(true_x - false_x) > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, V;
  cin >> n >> V;
  vector<array<int, 2>> va(n);
  for (auto &[v, a]: va) { cin >> v >> a; }

  auto f = [&](Int dlim) -> Int {
    auto dp = vector(2, vector(V + 1, array<Int, 2>{-1, 0}));
    dp[0][0] = {0, 0};
    REP(i, n) {
      auto[v, a] = va[i];
      dp[1] = dp[0];
      REP(j, V + 1) dp[1][j][1] = 0;
      REP(j, v, V + 1) {
        auto[b2, d2] = dp[1][j - v];
        if (b2 < 0 or d2 == -dlim) continue;
        chmax(dp[1][j], array<Int, 2>{b2 + a, d2 - 1});
      }
      swap(dp[0], dp[1]);
    }
    Int ret = 0;
    REP(j, V + 1) chmax(ret, dp[0][j][0]);
    return ret;
  };

  auto a0 = f(V + 5);
  auto x0 = bisect<Int>(V + 5, -1, [&](int x) { return f(x) == a0; });
  cout << a0 << ' ' << x0 << endl;
}
