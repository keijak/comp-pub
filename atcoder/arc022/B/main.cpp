#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}

using namespace std;

array<int, 100005> a;
array<int, 100005> prv;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  REP(i, n) {
    cin >> a[i];
    prv[a[i]] = -1;
  }

  int ans = 0, l = 0;
  REP(i, n) {
    int x = a[i];
    int p = prv[x];
    if (p >= 0) {
      l = min(l + 1, i - p);
    } else {
      l = l + 1;
    }
    chmax(ans, l);
    prv[x] = i;
  }

  cout << ans << '\n';
}
