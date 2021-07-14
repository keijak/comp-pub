#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
using namespace std;
using i64 = long long;

struct MaximizingGCD {
  int maximumGCDPairing(vector<int> A) {
    sort(A.begin(), A.end());
    i64 asum = accumulate(A.begin(), A.end(), 0LL);
    int n = A.size();
    vector<i64> divs;
    for (i64 x = 1; x * x <= asum; ++x) {
      if (asum % x != 0) continue;
      i64 y = asum / x;
      divs.push_back(x);
      if (y > x) divs.push_back(y);
    }
    sort(divs.begin(), divs.end());
    int m = divs.size();
    int i = m - 1;

    auto check = [&](int i) -> bool {
      i64 g = divs[i];
      vector<bool> used(n, false);
      map<int, deque<int>> mp;
      for (int j = 0; j < n; ++j) {
        int q = A[j] % g;
        mp[q].push_back(j);
      }
      vector<i64> sums;
      int j = n - 1;
      for (; j >= 0; --j) {
        if (used[j]) continue;
        used[j] = true;
        int q = A[j] % g;
        auto& dq = mp[q == 0 ? 0 : g - q];
        bool ok = false;
        while (not dq.empty()) {
          int k = dq.front();
          dq.pop_front();
          if (used[k]) continue;
          used[k] = true;
          ok = true;
          sums.push_back(A[j] + A[k]);
          break;
        }
        if (not ok) return false;
      }
      if (int(sums.size()) != n / 2) return false;
      i64 z = sums[0];
      for (int j = 1; j < int(sums.size()); ++j) {
        z = __gcd(z, sums[j]);
      }
      if (z != g) return false;
      return true;
    };

    for (; i >= 0; --i) {
      if (check(i)) return divs[i];
    }
    return 1;
  }
};

int main() {
  MaximizingGCD s;
  int a;
  a = s.maximumGCDPairing({5, 4, 13, 2});
  cerr << a << endl;
  assert(a == 6);

  a = s.maximumGCDPairing({26, 23});
  cerr << a << endl;
  assert(a == 49);

  a = s.maximumGCDPairing({100, 200, 300, 500, 1100, 700});
  cerr << a << endl;
  assert(a == 100);

  a = s.maximumGCDPairing(
      {46, 78, 133, 92, 1, 23, 29, 67, 43, 111, 3908, 276, 13, 359, 20, 21});
  cerr << a << endl;
  assert(a == 4);
}
