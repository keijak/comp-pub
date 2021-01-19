#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#define REP(i, n) for (int i = 0, n_ = (n); i < n_; ++i)

using i64 = long long;
using u64 = unsigned long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

using namespace std;

optional<vector<int>> solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> words(n);
  REP(i, n) {
    int l;
    cin >> l;
    words[i].resize(l);
    REP(j, l) cin >> words[i][j];
  }

  vector<vector<int>> g(m + 1);

  auto check_prefix = [&](const vector<int> &x, const vector<int> &y) -> bool {
    if (ssize(x) > ssize(y)) return false;
    REP(i, ssize(x)) {
      if (x[i] != y[i]) return false;
    }
    return true;
  };

  REP(i, n - 1) {
    if (check_prefix(words[i], words[i + 1])) continue;
    if (check_prefix(words[i + 1], words[i])) return nullopt;
    int l = min(ssize(words[i]), ssize(words[i + 1]));
    int pos = -1;
    REP(j, l) {
      if (words[i][j] != words[i + 1][j]) {
        pos = j;
        break;
      }
    }
    assert(pos != -1);
    int x = words[i][pos];
    int y = words[i + 1][pos];
    assert(x != y);
    g[x].push_back(y);  // x <= y
  }
  vector<bool> visiting(m + 1);
  vector<optional<bool>> memo(m + 1);
  auto need_capitalize = [&](auto self, int x) -> optional<bool> {
    if (memo[x].has_value()) return memo[x].value();
    if (visiting[x]) return nullopt;
    visiting[x] = true;

    bool cx = false;
    for (auto y : g[x]) {
      auto ry = self(self, y);
      if (!ry) return nullopt;
      bool cy = ry.value();
      if (x < y) {
        if (cy) cx = true;
      } else {
        if (cy) return nullopt;
        cx = true;
      }
    }
    memo[x] = cx;
    visiting[x] = false;
    return cx;
  };

  vector<int> ans;
  REP(i, m) {
    int x = i + 1;
    auto res = need_capitalize(need_capitalize, x);
    if (!res) return nullopt;
    if (res.value()) {
      ans.push_back(x);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (!res) {
    cout << "No\n";
  } else {
    cout << "Yes\n";
    auto vals = move(*res);
    cout << ssize(vals) << "\n";
    if (not vals.empty()) {
      REP(i, ssize(vals)) {
        if (i > 0) cout << " ";
        cout << vals[i];
      }
      cout << "\n";
    }
  }
}
