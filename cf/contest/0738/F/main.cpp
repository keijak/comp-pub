#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using i32 = std::int32_t;
using u32 = std::uint32_t;

using namespace std;

const int L = 4001;
const int K = 91;
const i64 INF = 1e18;
i64 memo1[K * K * L];
i64 memo2[K * K * L];

int n;
vector<i64> cum;

i64 minimizer(u32 l, i32 d, i32 k);

i64 maximizer(u32 l, i32 d, i32 k) {
  u32 r = n - l - d;
  if (l + k - 1 >= r) return 0;
  if (l + k == r) return cum[l + k] - cum[l];

  u32 key = l * K * K + d * K + k;
  auto &cached = memo1[key];
  if (cached != INF) return cached;

  i64 r1 = (cum[l + k] - cum[l]) + minimizer(l + k, d - k, k);
  i64 r2 = (cum[l + k + 1] - cum[l]) + minimizer(l + k + 1, d - k - 1, k + 1);
  i64 res = max(r1, r2);
  return cached = res;
}

i64 minimizer(u32 l, i32 d, i32 k) {
  u32 r = n - l - d;
  if (l + k - 1 >= r) return 0;
  if (l + k == r) return -(cum[r] - cum[r - k]);

  u32 key = l * K * K + (d + K) * K + k;
  auto &cached = memo2[key];
  if (cached != INF) return cached;

  i64 r1 = -(cum[r] - cum[r - k]) + maximizer(l, d + k, k);
  i64 r2 = -(cum[r] - cum[r - k - 1]) + maximizer(l, d + k + 1, k + 1);
  i64 res = min(r1, r2);
  return cached = res;
}

i64 solve() {
  cin >> n;
  cum.resize(n + 1);
  fill(ALL(memo1), INF);
  fill(ALL(memo2), INF);

  REP(i, n) {
    i64 x;
    cin >> x;
    cum[i + 1] = cum[i] + x;
  }
  return maximizer(0, 0, 1);
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
