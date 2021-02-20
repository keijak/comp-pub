#include <bits/stdc++.h>
using namespace std;

struct TopXorer {
  int maximalRating(vector<int> x) {
    sort(x.rbegin(), x.rend());
    unsigned ans = 0;
    for (int v : x) {
      for (int bit = 1U << 30; bit; bit >>= 1) {
        if (!(v & bit)) continue;
        if (ans & bit) {
          ans |= (bit - 1);
          break;
        } else {
          ans |= bit;
        }
      }
    }
    return ans;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  TopXorer solver;
  cout << solver.maximalRating({2, 2, 2}) << endl;
  cout << solver.maximalRating({1, 2, 4, 8, 16}) << endl;
  cout << solver.maximalRating({1024, 1024}) << endl;
  cout << solver.maximalRating({7, 4, 12, 33, 6, 8, 3, 1}) << endl;
  cout << solver.maximalRating({0}) << endl;
}
