#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
#define REP(i, n) for (int i = 0; i < (n); i++)

const long double EPS = 1e-9;

struct WaterTank {
  double minOutputRate(vector<int> t, vector<int> x, int C) {
    const int n = t.size();

    auto check = [&](long double R) -> bool {
      long double total = 0;
      REP(i, n) {
        total += (x[i] - R) * t[i];
        if (x[i] < R) {
          if (total < 0) total = 0;
        } else {
          if (total - EPS > C) return false;
        }
      }
      return true;
    };

    long double fv = 0.0, tv = 1e13;
    REP(i, 100) {
      long double mid = (fv + tv) * 0.5;
      if (check(mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  }
};

int main() {
  cout << fixed << setprecision(15);
  WaterTank s;
  { cout << s.minOutputRate({3, 3}, {1, 2}, 3) << endl; }

  { cout << s.minOutputRate({1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}, 10) << endl; }

  {
    cout << s.minOutputRate({9, 3, 4, 8, 1, 2, 5, 7, 6},
                            {123, 456, 789, 1011, 1213, 1415, 1617, 1819, 2021},
                            11)
         << endl;
  }
  { cout << s.minOutputRate({100}, {1000}, 12345) << endl; }
}
