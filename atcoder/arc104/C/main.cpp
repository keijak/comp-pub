#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = static_cast<int>(n); i < REP_N_; ++i)

using namespace std;

struct Floor {
  bool filled;
  bool start;           // true: get on, false: get off
  int person;           // who gets on/off?
  optional<int> width;  // where's the other end?
};

bool solve() {
  int n;
  cin >> n;
  const int n2 = n * 2;
  vector<Floor> floors(n2);
  REP(i, n) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    optional<int> width;
    if (a >= 0 and b >= 0) {
      if (a >= b) return false;
      width = b - a;
    }
    if (a >= 0) {
      if (floors[a].filled) return false;
      floors[a] = {true, true, i, width};
    }
    if (b >= 0) {
      if (floors[b].filled) return false;
      floors[b] = {true, false, i, width};
    }
  }

  auto is_ok_simple = [&](int l, int r) -> bool {
    const int width = (r - l) / 2;
    for (int i = l; i < l + width; ++i) {
      const Floor &f = floors[i];
      const Floor &other = floors[i + width];
      if (f.filled and other.filled) {
        if (not f.start or other.start) return false;
        if (f.person != other.person) return false;
        if (not f.width.has_value() or not other.width.has_value() or
            f.width.value() != width or other.width.value() != width) {
          return false;
        }
      } else if (f.filled) {
        if (not f.start) return false;
        if (f.width.has_value()) return false;
      } else if (other.filled) {
        if (other.start) return false;
        if (other.width.has_value()) return false;
      }
    }
    return true;
  };

  auto dp = vector(n2, vector(n2 + 1, optional<bool>()));

  auto is_ok = [&](auto self, int l, int r) -> bool {
    if (dp[l][r].has_value()) return dp[l][r].value();
    assert((r - l) % 2 == 0);
    if (is_ok_simple(l, r)) {
      dp[l][r] = true;
      return true;
    }
    // Split the interval.
    bool ok = false;
    for (int i = l + 2; i < r; i += 2) {
      ok = self(self, l, i) and self(self, i, r);
      if (ok) break;
    }
    dp[l][r] = ok;
    return ok;
  };

  return is_ok(is_ok, 0, n2);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << (solve() ? "Yes\n" : "No\n");
}
