#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

using namespace std;

const i64 INF = 1e18;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<pair<i64, i64>> bs(n);
  REP(i, n) { cin >> bs[i].first >> bs[i].second; }

  auto check = [&](i64 x) -> bool {
    map<i64, int> mp;
    REP(i, n) {
      auto [h, s] = bs[i];
      if (h > x) return false;
      i64 tmax = (x - h) / s;
      ++mp[tmax];
    }
    int count = 0;
    for (auto [tlimit, c] : mp) {
      count += c;
      if (count - 1 > tlimit) return false;
    }
    return true;
  };

  i64 fv = -1, tv = INF;
  while (tv - fv > 1) {
    i64 mid = (fv + tv) / 2;
    (check(mid) ? tv : fv) = mid;
  }
  cout << tv << endl;
}
