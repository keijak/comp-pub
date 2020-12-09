#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
using i64 = long long;

template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

using namespace std;

i64 solve() {
  int n;
  cin >> n;
  vector<i64> a(n), b(n);
  i64 total = 0, bmin = 1e15;
  bool all_eq = true;
  REP(i, n) {
    cin >> a[i] >> b[i];
    total += b[i];
    if (a[i] != b[i]) all_eq = false;
    if (a[i] > b[i]) chmin(bmin, b[i]);
  }
  if (all_eq) return 0;
  return total - bmin;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
