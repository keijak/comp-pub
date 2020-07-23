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
  int n, ga, gb, gc;
  cin >> n >> ga >> gb >> gc;
  vector<int> lens(n);
  REP(i, n) cin >> lens[i];

  i64 minmp = ga + gb + gc;
  vector<int> tmp(n, -1);

  auto calc = [&](auto rec, int i, int ta, int tb, int tc, i64 mp) -> void {
    if (mp > minmp) return;
    if (i == n) {
      if (ta == 0 || tb == 0 || tc == 0) return;
      mp += abs(ta - ga) + abs(tb - gb) + abs(tc - gc);
      if (minmp > mp) {
        minmp = mp;
      }
      return;
    }
    int x = lens[i];
    tmp[i] = 0;
    rec(rec, i + 1, ta, tb, tc, mp);
    if (ta < ga) {
      tmp[i] = 1;
      rec(rec, i + 1, ta + x, tb, tc, ta > 0 ? (mp + 10) : mp);
    }
    if (tb < gb) {
      tmp[i] = 2;
      rec(rec, i + 1, ta, tb + x, tc, tb > 0 ? (mp + 10) : mp);
    }
    if (tc < gc) {
      tmp[i] = 3;
      rec(rec, i + 1, ta, tb, tc + x, tc > 0 ? (mp + 10) : mp);
    }
  };
  calc(calc, 0, 0, 0, 0, 0);
  cout << minmp << endl;
}
