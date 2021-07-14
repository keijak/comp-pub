#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

const int INF = 1e5 + 100;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, X;
  cin >> N >> X;
  V<int> T(N);
  V<pair<i64, i64>> food(N);
  REP(i, N) cin >> food[i].second;
  i64 asum = 0;
  REP(i, N) {
    cin >> food[i].first;
    asum += food[i].first;
  }
  if (asum < X) {
    cout << -1 << endl;
    return 0;
  }
  sort(food.rbegin(), food.rend());

  auto check = [&](int tlimit) -> bool {
    set<int> slots;
    REP(i, tlimit) slots.insert(i);
    i64 bs = 0;
    REP(i, N) {
      auto [a, t] = food[i];
      auto it = slots.lower_bound(t);
      if (it == slots.begin()) continue;
      --it;
      bs += a;
      if (bs >= X) return true;
      slots.erase(it);
    }
    return false;
  };

  int ng = 0, ok = INF;
  while (ok - ng > 1) {
    int mid = (ng + ok) / 2;
    if (check(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }
  if (ok == INF) {
    cout << -1 << endl;
  } else {
    cout << ok << endl;
  }
}
