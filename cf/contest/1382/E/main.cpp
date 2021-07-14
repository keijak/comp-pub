#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
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

void solve() {
  int n, x, y;
  cin >> n >> x >> y;
  set<int> unused_colors;
  REP(i, n + 1) { unused_colors.insert(i); }
  vector<int> b(n);
  REP(i, n) {
    cin >> b[i];
    b[i]--;
    unused_colors.erase(b[i]);
  }

  vector<vector<int>> color_pos(n + 1);
  REP(i, n) { color_pos[b[i]].push_back(i); }

  priority_queue<tuple<int, int>> color_q;
  REP(c, n + 1) { color_q.emplace(color_pos[c].size(), c); }

  vector<bool> fixed(n);
  int to_fix = x;
  while (to_fix > 0 && !color_q.empty()) {
    auto [c1size, c1] = color_q.top();
    color_q.pop();
    if (color_q.empty()) {
      assert(c1size >= to_fix);
      REP(i, to_fix) {
        int j = color_pos[c1].back();
        fixed[j] = true;
        color_pos[c1].pop_back();
      }
      to_fix = 0;
      break;
    }
    auto [c2size, c2] = color_q.top();
    int z = min(c1size - c2size + 1, to_fix);
    REP(i, z) {
      int j = color_pos[c1].back();
      fixed[j] = true;
      color_pos[c1].pop_back();
    }
    to_fix -= z;
    color_q.emplace(color_pos[c1].size(), c1);
  }

  int rot_width = 0;
  if (!color_q.empty()) {
    rot_width = get<0>(color_q.top());
  }

  vector<tuple<int, int>> cpos;
  while (color_q.size()) {
    auto [csize, c] = color_q.top();
    color_q.pop();
    assert(color_pos[c].size() == csize);
    REP(i, csize) { cpos.emplace_back(c, color_pos[c][i]); }
  }
  assert(cpos.size() == n - x);

  int replace_count = n - y;
  vector<tuple<int, int>> cpos2 = cpos;
  int unused_color = *unused_colors.begin();
  REP(i, cpos2.size()) {
    auto [c1, j1] = cpos[(i + rot_width) % cpos.size()];
    auto [c2, j2] = cpos[i];
    if (c1 != c2) {
      cpos2[i] = {c2, j1};
    } else if (replace_count > 0) {
      replace_count--;
      DEBUG(replace_count, unused_color, j1);
      cpos2[i] = {unused_color, j1};
    } else {
      cout << "NO\n";
      return;
    }
  }

  vector<int> ans(n, -1);
  REP(i, n) {
    if (fixed[i]) {
      ans[i] = b[i] + 1;
      DEBUG("fixed", i, ans[i]);
    }
  }
  REP(i, cpos2.size()) {
    auto [c, j] = cpos2[i];
    assert(ans[j] == -1);
    if (c != unused_color && replace_count > 0) {
      replace_count--;
      ans[j] = unused_color + 1;
    } else {
      ans[j] = c + 1;
    }
  }
  assert(replace_count == 0);
  cout << "YES\n";
  REP(i, n) { cout << ans[i] << (i == n - 1 ? "\n" : " "); }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    DEBUG(t);
    solve();
  }
}
