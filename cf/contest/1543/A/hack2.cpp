#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
void solve() {
  ll a, b;
  cin >> a >> b;
  if (a == b) {
    cout << 0 << " " << 0 << endl;
    return;
  }
  ll temp = abs(a - b);
  ll moda = ((a / temp) + 1) * temp;
  ll modb = ((b / temp) + 1) * temp;
  ll backa = (a / temp) * temp;
  ll backb = (b / temp) * temp;
  ll ans1 = __gcd(moda, modb);
  ll ans2 = max(a, b) - min(a, b);
  if (ans1 > ans2) {
    cout << ans1 << " " << min(moda - a, a - backa) << endl;
  } else if (ans1 == ans2) {
    cout << ans1 << " " << min(min(moda - a, a - backa), min(a, b)) << endl;
  } else {
    cout << ans2 << " " << min(a, b) << endl;
  }
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}
