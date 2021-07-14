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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<int> colors(N);
  REP(i, N) cin >> colors[i];

  int bsize = sqrt(N + 1) + 1;
  V<int> bucket(N);
  {
    int i = 0, k = 1;
    for (; i < N; ++k) {
      for (; i < N && i < k * bsize; ++i) {
        bucket[i] = k - 1;
      }
    }
  }

  V<tuple<int, int, int, int, int>> queries(Q);
  REP(i, Q) {
    int l, r;
    cin >> l >> r;
    l--;
    queries[i] = {bucket[l], bucket[r - 1], l, r, i};
  }
  sort(queries.begin(), queries.end());

  V<int> counter(500005);
  int cl = 0, cr = 0, ucount = 0;  //  unique count in [cl, cr).
  V<int> ans(Q);
  REP(i, Q) {
    auto [lb, rb, l, r, q] = queries[i];
    for (; cr < r; cr++) {
      if (counter[colors[cr]]++ == 0) ucount++;
    }
    for (; cl > l;) {
      --cl;
      if (counter[colors[cl]]++ == 0) ucount++;
    }
    for (; cr > r;) {
      --cr;
      if (--counter[colors[cr]] == 0) --ucount;
    }
    for (; cl < l; cl++) {
      if (--counter[colors[cl]] == 0) --ucount;
    }
    ans[q] = ucount;
  }
  REP(q, Q) { cout << ans[q] << '\n'; }
}
