#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

#define chmin(x, y) x = min(x, y)

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  V<string> lines(n);
  REP(i, n) {
    cin >> lines[i];
    assert(lines[i].size() == size_t(m));
  }
  auto color_of = [&](int r, int c) -> int { return lines[r][c] - 'a'; };

  V<V<int>> cum_up(n + 1, V<int>(m));
  for (int i = 1; i <= n; ++i) {
    REP(j, m) {
      int color = color_of(i - 1, j);
      int x = 0;
      if (i > 1 && color == color_of(i - 2, j)) {
        x = cum_up[i - 1][j];
      }
      cum_up[i][j] = x + 1;
    }
  }

  V<V<int>> cum_lu(n + 1, V<int>(m + 1));
  for (int r = 1; r <= n; ++r) {
    for (int c = 1; c <= m; ++c) {
      int color = color_of(r - 1, c - 1);
      int x = 0;
      if (r - 2 >= 0 && c - 2 >= 0 && color == color_of(r - 2, c - 2)) {
        x = cum_lu[r - 1][c - 1];
      }
      int lim = (cum_up[r][c - 1] + 1) / 2;
      cum_lu[r][c] = min(x + 1, lim);
    }
  }

  V<V<int>> cum_ru(n + 1, V<int>(m + 1));
  for (int r = 1; r <= n; ++r) {
    for (int c = m - 1; c >= 0; --c) {
      int color = color_of(r - 1, c);
      int x = 0;
      if (r - 2 >= 0 && c + 1 < m && color == color_of(r - 2, c + 1)) {
        x = cum_ru[r - 1][c + 1];
      }
      int lim = (cum_up[r][c] + 1) / 2;
      cum_ru[r][c] = min(x + 1, lim);
    }
  }

  i64 ans = 0;
  REP(r, n) {
    REP(c, m) {
      // int color = lines[r][c] - 'a';
      int cnt = n + 100;
      int ux = cum_up[r + 1][c];
      int up = (ux + 1) / 2;
      int lu = cum_lu[r + 1][c + 1];
      int ru = cum_ru[r + 1][c];
      chmin(cnt, up);
      chmin(cnt, lu);
      chmin(cnt, ru);
      if (cnt < 1) cnt = 1;
      DEBUG(r, c, lines[r][c], cnt, ux, up, lu, ru);
      ans += cnt;
    }
  }

  cout << ans << endl;
}
