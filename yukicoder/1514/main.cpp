#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

char p[50000005];
int b[50000005];

auto solve() {
  i64 n;
  cin >> n;
  fill(ALL(p), 1);
  //   vector<char> p(n + 1, true);
  //   vector<int> b(n + 1);
  p[0] = p[1] = false;
  REP(i, n + 1) b[i] = i;
  for (int i = 4; i <= n; i += 2) {
    p[i] = false;
    while ((b[i] & 3) == 0) b[i] >>= 2;
  }
  for (i64 i = 3; i * i <= n; i += 2) {
    if (not p[i]) continue;
    i64 ii = i * i;
    for (int j = ii; j <= n; j += ii) {
      p[j] = false;
      while ((b[j] % ii) == 0) b[j] /= ii;
    }
  }
  i64 ans = 0;
  for (i64 x = 1; x <= n; ++x) {
    i64 r = std::sqrt(n / b[x]);
    ans += r;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
