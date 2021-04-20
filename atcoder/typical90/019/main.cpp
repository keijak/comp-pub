#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

using namespace std;

i64 solve() {
  int n;
  cin >> n;
  const int n2 = n * 2;
  vector<i64> a(n2);
  cin >> a;
  vector dp(n2 + 1, vector(n2 + 1, -1LL));
  auto f = [&](auto &f, int l, int r) -> i64 {
    if (l >= r) return 0;
    auto &mem = dp[l][r];
    if (mem >= 0) return mem;
    const i64 x = a[l];
    i64 ans = 1e18;
    for (int j = l + 1; j < r; j += 2) {
      const i64 y = a[j];
      const i64 cost = abs(x - y) + f(f, l + 1, j) + f(f, j + 1, r);
      chmin(ans, cost);
    }
    return (mem = ans);
  };
  return f(f, 0, n2);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
