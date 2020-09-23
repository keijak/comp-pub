#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const i64 INF = 1LL << 40;

using VP = vector<tuple<i64, i64>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  VP pu, pr, pd, pl;
  REP(i, N) {
    i64 x, y;
    char u;
    cin >> x >> y >> u;
    if (u == 'R') {
      pr.emplace_back(x, y);
    } else if (u == 'L') {
      pl.emplace_back(x, y);
    } else if (u == 'U') {
      pu.emplace_back(x, y);
    } else {
      assert(u == 'D');
      pd.emplace_back(x, y);
    }
  }
  i64 ans = INF;

  // Rotates points by k*90 degrees.
  auto rotate = [&](const VP& p, int k) -> VP {
    VP res(p.size());
    REP(i, p.size()) {
      auto [x, y] = p[i];
      REP(j, k) {
        i64 tmp = x;
        x = -y;
        y = tmp;
      }
      res[i] = {x, y};
    }
    return res;
  };

  auto check_du = [&](const VP& pd, const VP& pu) {
    map<i64, set<i64>> t;
    for (auto [x, y] : pd) {
      t[x].insert(y);
    }
    for (auto [x, y] : pu) {
      auto& s = t[x];
      auto it = s.lower_bound(y);
      if (it == s.end()) continue;
      ans = min(ans, (*it - y) * 5);
    }
  };

  auto check_dr = [&](const VP& pd, const VP& pr) {
    map<i64, set<i64>> t;
    for (auto [x, y] : pd) {
      t[y - x].insert(x);
    }
    for (auto [x, y] : pr) {
      auto& s = t[y - x];
      auto it = s.lower_bound(x);
      if (it == s.end()) continue;
      ans = min(ans, (*it - x) * 10);
    }
  };

  check_du(pd, pu);
  check_du(rotate(pl, 1), rotate(pr, 1));
  check_dr(pd, pr);
  check_dr(rotate(pl, 1), rotate(pd, 1));
  check_dr(rotate(pu, 2), rotate(pl, 2));
  check_dr(rotate(pr, 3), rotate(pu, 3));

  if (ans == INF) {
    cout << "SAFE" << endl;
  } else {
    cout << ans << endl;
  }
}
