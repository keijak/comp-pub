#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const int B = 400;
int board[B * (B + 1) / 2 + 5];

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;

  vector<pair<i64, i64>> trains(n);
  for (auto&[x, y]: trains) {
    cin >> x >> y;
  }
  vector<optional<pair<int, int>>> small_queries(m);
  vector<vector<pair<int, int>>> big_queries(n);
  REP(d, m) {
    int op, t;
    cin >> op >> t;
    --t;
    const auto&[x, y] = trains[t];
    const i64 z = x + y;
    if (z >= B) {
      big_queries[t].push_back({d, op});
    } else {
      small_queries[d] = {op, t};
    }
  }

  vector<i64> timeline(m + 5);
  REP(k, n) {
    auto &v = big_queries[k];
    if (v.empty()) continue;
    if (int(v.size()) & 1) {
      v.push_back({m, 2});
    }
    const auto&[x, y] = trains[k];
    const i64 z = x + y;
    for (int i = 0; i < int(v.size()); i += 2) {
      const auto&[d1, op1] = v[i];
      const auto&[d2, op2] = v[i + 1];
//      assert(op1 == 1);
//      assert(op2 == 2);
      for (int j = d1; j < d2; j += z) {
        if (j + x >= d2) {
          continue;
        }
        timeline[j + x]++;
        if (j + z < d2) {
          timeline[j + z]--;
        } else {
          timeline[d2]--;
        }
      }
    }
  }
  big_queries.clear();
  REP(i, m) timeline[i + 1] += timeline[i];

  vector<int> last_added(n);
  for (i64 d = 0; d < m; ++d) {
    const auto &query = small_queries[d];
    if (query) {
      auto[op, t] = *query;
      const auto&[x, y] = trains[t];
      const i64 z = x + y;
      const i64 z2 = z * (z - 1) / 2;
      int *bp = board + z2;
      if (op == 1) {
        last_added[t] = d;
        if (d + x < m) {
          i64 q = (d + x) % z;
          REP(j, y) {
            if (q >= z) q -= z;
            ++bp[q++];
          }
        }
      } else {
        int d0 = last_added[t];
        i64 q = (d0 + x) % z;
        REP(j, y) {
          if (q >= z) q -= z;
          --bp[q++];
        }
      }
    }
    i64 ans = timeline[d];
    REP(j, 2, B) {
      int z2 = j * (j - 1) / 2;
      ans += board[z2 + (d % j)];
    }
    cout << ans << "\n";
  }
}
