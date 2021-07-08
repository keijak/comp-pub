#include <iostream>
using namespace std;
#include <math.h>

#include <algorithm>

int main() {
  int t;
  cin >> t;
  while (t--) {
    long long int a, b, count1 = 0, count2 = 0;
    cin >> a >> b;
    long long int ans = abs(a - b);
    if (ans > 1) {
      count1 = ans - (a % ans);
      count2 = a % ans;
    }
    cout << ans << " " << min(count1, count2) << endl;
  }
}