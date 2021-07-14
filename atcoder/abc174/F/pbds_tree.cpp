#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template <class Key>
using rb_tree_set =
    __gnu_pbds::tree<Key, __gnu_pbds::null_type, std::less<Key>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<int> colors(N);
  REP(i, N) cin >> colors[i];
  vector<int> lefts(Q), rights(Q), ids(Q);
  REP(i, Q) {
    cin >> lefts[i] >> rights[i];
    --lefts[i];
    --rights[i];
    ids[i] = i;
  }
  sort(ids.begin(), ids.end(),
       [&](int i, int j) { return rights[i] < rights[j]; });

  rb_tree_set<pair<int, int>> st;
  vector<int> prev(500'005, -1);
  vector<int> res(Q);
  int j = 0;
  for (int i = 0; i < N; ++i) {
    int pc = prev[colors[i]];
    if (pc >= 0) st.insert({pc, i});
    for (; j < Q && rights[ids[j]] == i; ++j) {
      auto it = st.lower_bound(make_pair(lefts[ids[j]], -1));
      int dup_count = 0;
      if (it != st.end()) {
        dup_count = st.size() - st.order_of_key(*it);
      }
      res[ids[j]] = rights[ids[j]] - lefts[ids[j]] + 1 - dup_count;
      DEBUG(j, ids[j], lefts[ids[j]], rights[ids[j]], dup_count, res[ids[j]]);
    }
    prev[colors[i]] = i;
  }
  for (int i = 0; i < Q; ++i) cout << res[i] << '\n';
}
