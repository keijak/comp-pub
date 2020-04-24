#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; ++i)

using i64 = long long;
const int INF = 1000000;

const char *segs[10] = {
    "1110111", "0010010", "1011101", "1011011", "0111010",
    "1101011", "1101111", "1010010", "1111111", "1111011",
};

int diff(string_view s1, string_view s2) {
  int cnt = 0;
  rep(i, 7) {
    if (s1[i] == s2[i]) continue;
    if (s1[i] == '1' && s2[i] == '0') {
      cnt += 1;
    } else {
      return INF;
    }
  }
  return cnt;
}

int main() {
  int n, k;
  cin >> n >> k;
  string s;
  getline(cin, s);  // skip to the next line.

  vector<string> bins(n);
  rep(i, n) { getline(cin, bins[i]); }
  vector<int> maxdiff(n, INF);
  vector<int> mindiff(n, 0);
  rep(i, n) {
    rep(j, 10) {
      auto d = diff(segs[j], bins[i]);
      mindiff[i] = min(mindiff[i], d);
      maxdiff[i] = max(maxdiff[i], d);
    }
  }
  vector<int> acummin(n, 0);
  vector<int> acummax(n, 0);
  for (int i = n - 2; i >= 0; --i) {
    acummin[i] = acummin[i + 1] + mindiff[i + 1];
    acummax[i] = acummax[i + 1] + maxdiff[i + 1];
  }
  vector<int> ans(n, 0);
  vector<vector<bool>> dp(n + 1, vector<bool>(k + 1, true));
  function<bool(int, int)> solve = [&](int i, int sticks) {
    if (!dp[i][sticks]) return false;
    if (i == n) {
      if (sticks == 0) {
        for (auto v : ans) {
          cout << v;
        }
        cout << endl;
        return true;
      }
      return false;
    }
    auto &digit = bins[i];
    for (int j = 9; j >= 0; --j) {
      auto sd = diff(segs[j], digit);
      int rem = sticks - sd;
      if (acummin[i] <= rem && rem <= acummax[i]) {
        ans[i] = j;
        if (solve(i + 1, rem)) {
          return true;
        }
      }
    }
    dp[i][sticks] = false;
    return false;
  };
  if (!solve(0, k)) {
    cout << -1 << endl;
  }
}
