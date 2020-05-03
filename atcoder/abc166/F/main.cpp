#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

const string YES = "Yes";
const string NO = "No";

int main() {
  int64 N, A, B, C;
  std::vector<std::string> s;
  cin >> N >> A >> B >> C;
  vector<int64> buf{A, B, C};
  s = vector<string>(N);
  for (int i = 0; i < N; i++) {
    std::cin >> s[i];
  }
  vector<char> ans;
  auto dfs = [&](auto rec, int idx) -> bool {
    if (idx == N) {
      cout << YES << "\n";
      REP(i, N) { cout << ans[i] << "\n"; }
      return true;
    }
    int x = s[idx][0] - 'A';
    int y = s[idx][1] - 'A';
    REP(_, 2) {
      if (buf[x] > 0) {
        buf[x]--;
        buf[y]++;
        ans.push_back(y + 'A');
        if (rec(rec, idx + 1)) {
          return true;
        };
        ans.pop_back();
        buf[x]++;
        buf[y]--;
      }
      swap(x, y);
    }
    return false;
  };
  if (!dfs(dfs, 0)) {
    cout << NO << endl;
  }
}
