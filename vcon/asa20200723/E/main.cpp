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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<tuple<int, int>> ans(100);
  int cumx = 0, cumy = 0;
  for (int i = 1; i <= 100; ++i) {
    if (cumx + 2 * i > 1500) {
      cumy += 2 * i;
      cumx = 0;
    }
    ans[i - 1] = {cumx + i, cumy + i};
    assert(cumx + 2 * i <= 1500);
    assert(cumy + 2 * i <= 1500);
    cumx += 2 * i;
  }

  REP(i, 100) {
    auto [x, y] = ans[i];
    cout << x << " " << y << '\n';
  }
}
