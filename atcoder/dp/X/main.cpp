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
  int N;
  cin >> N;
  vector<tuple<int, int, i64>> blocks(N);
  REP(i, N) {
    int w, s, v;
    cin >> w >> s >> v;
    blocks[i] = {w, s, v};
  }
  sort(blocks.begin(), blocks.end());

  map<i64, i64> dp;
  dp[0] = 0;
  i64 ans = 0;
  REP(i, N) {
    auto [w, s, v] = blocks[i];
    auto it = dp.upper_bound(s);
    assert(it != dp.begin());
    --it;
    i64 new_value = it->second + v;
    int new_weight = it->first + w;

    auto it2 = dp.upper_bound(new_weight);
    assert(it2 != dp.begin());
    --it2;

    int prev_weight = it->first;
    int prev_value = it->second;
    DEBUG(prev_weight, prev_value, new_weight, new_value);

    if (it->second < new_value) {
      dp[new_weight] = new_value;
    }

    ans = max(ans, new_value);
  }
  cout << ans << endl;
}
