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
  int N, A;
  cin >> N >> A;
  vector<int> X(N);
  for (auto& x : X) cin >> x;
  sort(X.begin(), X.end());
  vector<int> cum(N + 1);
  for (int i = N - 1; i >= 0; --i) {
    cum[i] = cum[i + 1] + X[i];
  }
  map<tuple<int, int, int>, i64> memo;
  auto dfs = [&](auto rec, int i, int num, int acc) -> i64 {
    i64 res = 0;
    if (i >= N) return res;
    if (acc > N * A) return res;
    tuple<int, int, int> key = {i, num, acc};
    auto it = memo.find(key);
    if (it != memo.end()) {
      return it->second;
    }
    res += rec(rec, i + 1, num, acc);
    if (acc + X[i] == (num + 1) * A) {
      res++;
    }
    res += rec(rec, i + 1, num + 1, acc + X[i]);
    memo[key] = res;
    return res;
  };
  i64 ans = dfs(dfs, 0, 0, 0);
  cout << ans << '\n';
}