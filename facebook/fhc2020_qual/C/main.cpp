#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>

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

template <class Key, class T>
using gp_hash_table = __gnu_pbds::gp_hash_table<Key, T>;

i64 solve() {
  int n;
  cin >> n;
  vector<tuple<i64, i64>> trees(n);
  i64 maxh = 0;
  REP(i, n) {
    i64 p, h;
    cin >> p >> h;
    trees[i] = {p, h};
    maxh = max(maxh, h);
  }
  sort(trees.begin(), trees.end());
  unordered_map<i64, i64> max_interval;
  i64 ans = 0;
  for (auto [p, h] : trees) {
    i64 t = h + max_interval[p];
    ans = max(ans, t);
    max_interval[p + h] = max(max_interval[p + h], t);
    t = h + max_interval[p - h];
    ans = max(ans, t);
    max_interval[p] = max(max_interval[p], t);
  }
  assert(ans >= maxh);
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << "Case #" << (i + 1) << ": " << solve() << "\n"; }
}
