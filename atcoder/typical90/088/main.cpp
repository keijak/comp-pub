#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

using namespace std;
using BS = std::bitset<88>;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, Q;
  cin >> n >> Q;
  vector<unsigned> a(n);
  for (auto &x : a) cin >> x;
  vector<BS> ban(n);
  REP(i, Q) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    ban[y].set(x, true);
  }

  auto set_pair = [&]() -> array<BS, 2> {
    const int W = 8889;
    auto dp = vector(W, optional<BS>());
    const BS empty_set;
    dp[0] = empty_set;

    queue<tuple<int, int, BS>> q;
    q.push({0, 0, empty_set});
    while (not q.empty()) {
      auto[i, w, s1] = move(q.front());
      q.pop();
      if (i + 1 < n) {
        q.push({i + 1, w, s1});
      }
      const unsigned x = a[i];
      if (w + x >= W) continue;
      if ((ban[i] & s1).any()) continue;
      s1.set(i, true);
      if (auto &s2 = dp[w + x]; s2) {
        return {s1, *s2};
      }
      dp[w + x] = s1;
      if (i + 1 < n) {
        q.push({i + 1, w + x, s1});
      }
    }
    assert(false);
  }();

  for (auto &s : set_pair) {
    cout << s.count() << '\n';
    bool first = true;
    REP(i, 88) {
      if (s[i]) {
        if (not first) cout << ' ';
        else first = false;
        cout << i + 1;
      }
    }
    cout << '\n';
  }
}
