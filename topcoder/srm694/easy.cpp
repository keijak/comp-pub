#include <bits/stdc++.h>
using namespace std;

#define REP(i, n) for (int i = 0; i < (n); i++)

template <class T>
void chmax(T& a, const T& b) {
  a = max(a, b);
}

const int M = 256;
const int BIG = 1e8;

struct TrySail {
  int get(vector<int> strength) {
    const int n = strength.size();

    unsigned allx = 0;

    vector<vector<vector<vector<int>>>> dp(
        2, vector<vector<vector<int>>>(
               M, vector<vector<int>>(M, vector<int>(8, -BIG))));
    dp[0][0][0][0] = 0;

    REP(i, n) {
      int x = strength[i];
      allx ^= strength[i];

      REP(j, M) REP(k, M) REP(p, 8) {
        if (dp[0][j][k][p] < 0) continue;
        {
          // first team
          int j2 = j ^ x;
          chmax(dp[1][j2][k][p | 1], dp[0][j][k][p] - j + j2);
        }
        {
          // second team
          int k2 = k ^ x;
          chmax(dp[1][j][k2][p | 2], dp[0][j][k][p] - k + k2);
        }
        {
          // third team
          int z2 = allx ^ j ^ k;
          int z = z2 ^ x;
          chmax(dp[1][j][k][p | 4], dp[0][j][k][p] - z + z2);
        }
      }
      swap(dp[0], dp[1]);
      REP(j, M) REP(k, M) REP(p, 8) { dp[1][j][k][p] = -BIG; }
    }

    int ans = 0;
    REP(j, M) REP(k, M) { chmax(ans, dp[0][j][k][7]); }
    return ans;
  }
};

int main() {
  TrySail s;
  int a;
  {
    a = s.get({2, 3, 3});
    cerr << a << endl;
    assert(a == 8);
  }
  {
    a = s.get({7, 3, 5, 2});
    cerr << a << endl;
    assert(a == 17);
  }
  {
    a = s.get({13, 13, 13, 13, 13, 13, 13, 13});
    cerr << a << endl;
    assert(a == 26);
  }
  {
    a = s.get({114, 51, 4, 191, 9, 81, 0, 89, 3});
    cerr << a << endl;
    assert(a == 470);
  }
  {
    a = s.get({108, 66,  45,  82,  163, 30, 83,  244, 200, 216, 241, 249, 89,
               128, 36,  28,  250, 190, 70, 95,  117, 196, 19,  160, 255, 129,
               10,  109, 189, 24,  22,  25, 134, 144, 110, 15,  235, 205, 186,
               103, 116, 191, 119, 183, 45, 217, 156, 44,  97,  197});
    cerr << a << endl;
    assert(a == 567);
  }
}