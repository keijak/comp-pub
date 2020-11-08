#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}

using namespace std;

array<int, 100005> a;
bitset<100005> b;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  REP(i, n) cin >> a[i];

  int r = 0, ans = 0;
  REP(l, n) {
    for (; r < n; ++r) {
      int x = a[r];
      if (b[x]) break;
      b.set(x, true);
    }
    chmax(ans, r - l);
    b.set(a[l], false);
  }
  cout << ans << '\n';
}
