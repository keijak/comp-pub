#include <bits/stdc++.h>
using Int = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}

using namespace std;

Int solve() {
  Int W, N;
  cin >> W >> N;
  vector<Int> dp1(W + 1, -1LL);
  dp1[0] = 0;
  for (int i = 0; i < N; ++i) {
    Int l, r, value;
    cin >> l >> r >> value;
    vector<vector<pair<Int, bool>>> tmp(W + 2);
    for (int j = 0; j <= W; ++j) {
      if (j + l > W) break;
      if (dp1[j] < 0) continue;
      auto new_value = dp1[j] + value;
      tmp[j + l].push_back({new_value, true});
      tmp[min(j + r, W) + 1].push_back({new_value, false});
    }
    multiset<Int> ms;
    for (int j = 0; j <= W; ++j) {
      for (const auto &[val, push] : tmp[j]) {
        if (push) {
          ms.insert(val);
        } else {
          ms.erase(ms.find(val));
        }
      }
      if (not ms.empty()) {
        chmax(dp1[j], *ms.rbegin());
      }
    }
  }
  return dp1[W];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
