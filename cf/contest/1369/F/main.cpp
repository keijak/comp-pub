#define ENABLE_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  vector<i64> starts(T), ends(T);

  REP(i, T) { cin >> starts[i] >> ends[i]; }
  vector<bool> winnable(T + 1), losable(T + 1);
  winnable[T] = true;
  losable[T] = true;
  for (int i = T - 1; i >= 0; --i) {
    map<i64, bool> wins;
    i64 start = starts[i];
    auto can_win = [&](auto rec, int state) -> bool {
      if (state > ends[i]) return false;
      if (state == ends[i]) return true;
      bool res = !rec(rec, state * 2);
      if (res) {
        wins[state] = true;
        return true;
      }
      res = !rec(rec, state + 1);
      wins[state] = res;
      return res;
    };
    map<i64, bool> loses;
    auto can_lose = [&](auto rec, int state) -> bool {
      if (state > ends[i]) return true;
      if (state == ends[i]) return false;
      bool res = !rec(rec, state * 2);
      if (res) {
        loses[state] = true;
        return true;
      }
      res = !rec(rec, state + 1);
      loses[state] = res;
      return res;
    };
    bool w = can_win(can_win, start);
    bool l = can_lose(can_lose, start);
    winnable[i] = (w && winnable[i + 1]) || (l && !winnable[i + 1]);
    losable[i] = (l && losable[i + 1]) || (w && !losable[i + 1]);
  }
  cout << int(winnable[0]) << ' ' << int(losable[0]) << '\n';
}