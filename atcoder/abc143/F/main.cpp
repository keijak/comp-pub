#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> a(n);
  REP(i, n) cin >> a[i];

  map<int, int> counter;
  for (auto x : a) counter[x]++;
  vector<i64> freq;
  for (auto [x, cnt] : counter) freq.push_back(cnt);
  while (int(freq.size()) < n) freq.push_back(0);
  sort(ALL(freq));

  vector<i64> rcum(n + 1);
  for (int i = n - 1; i >= 0; --i) {
    rcum[i] = rcum[i + 1] + freq[i];
  }

  auto fill_amount = [&](int l, int r) -> i64 {
    i64 before = rcum[l] - rcum[r];
    i64 after = i64(r - l) * freq[r];
    return after - before;
  };

  auto find_tail = [&](int i, i64 rest) -> int {
    if (rest >= fill_amount(i, n - 1)) {
      return n - 1;
    }
    int tv = i, fv = n - 1;
    while (fv - tv > 1) {
      int mid = (fv + tv) / 2;
      if (rest >= fill_amount(i, mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  };

  vector<i64> ans(n + 1);
  ans[n] = freq[0];
  i64 rest = freq[0];
  REP(i, 1, n) {
    i64 k = n - i;
    int j = find_tail(i, rest);
    i64 fill1 = fill_amount(i, j);
    assert(rest >= fill1);
    i64 rest2 = rest - fill1;
    i64 h = rest2 / (j - i + 1);
    ans[k] = freq[j] + h;
    rest += freq[i];
  }
  REP(i, n) { cout << ans[i + 1] << "\n"; }
}
