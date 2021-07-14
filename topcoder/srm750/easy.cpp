#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct IdenticalBags {
  long long makeBags(vector<long long> candy, long long bagSize) {
    int n = candy.size();

    auto check = [&](i64 x) -> bool {
      i64 count = 0;
      for (int i = 0; i < n; ++i) {
        i64 k = candy[i] / x;
        count += k;
        if (count >= bagSize) return true;
      }
      return false;
    };
    i64 tv = 0, fv = i64(2e18) + 5;
    while (fv - tv > 1) {
      i64 mid = (tv + fv) / 2;
      (check(mid) ? tv : fv) = mid;
    }
    return tv;
  }
};

int main() {
  IdenticalBags s;
  i64 a;
  {
    a = s.makeBags({10, 11, 12}, 3);
    cerr << a << endl;
    assert(a == 10);
  }
}
