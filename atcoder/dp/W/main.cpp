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

template <class Monoid>
struct SegTree {
  using Func = function<Monoid(Monoid, Monoid)>;
  const Func F;
  const Monoid UNITY;
  int size;
  vector<Monoid> dat;

  SegTree(int n, const Func f, const Monoid &unity)
      : F(f), UNITY(unity), size(n), dat(2 * n, unity) {}

  // Sets i-th value (0-indexed) to x for initial setup.
  // build() must be called after set() calls.
  void set(int i, const Monoid &x) { dat[size + i] = x; }
  void build() {
    for (int k = size - 1; k > 0; --k) {
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const Monoid &x) {
    int k = size + i;
    dat[k] = x;
    while (k > 1) {
      k >>= 1;
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, open interval).
  Monoid fold(int l, int r) {
    l += size;
    r += size;
    Monoid vleft = UNITY, vright = UNITY;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = F(vleft, dat[l++]);
      if (r & 1) vright = F(dat[--r], vright);
    }
    return F(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  Monoid operator[](int i) { return dat[size + i]; }

  /* debug */
  void print() {
    for (int i = 0; i < size; ++i) {
      cout << (*this)[i];
      if (i != size - 1) cout << ",";
    }
    cout << endl;
  }
};

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<tuple<int, int, i64>> vt(m);
  i64 total_score = 0;
  REP(i, m) {
    i64 l, r, a;
    cin >> l >> r >> a;
    --l;
    vt[i] = {l, r, a};
    total_score += a;
  }
  sort(vt.begin(), vt.end());
  MinHeap<tuple<int, int, i64>> heap;
  i64 heap_score = 0;
  int cur = 0;
  vector<vector<i64>> dp(2, vector<i64>(n + 1));
  dp[0][0] = dp[1][0] = 0;
  for (int i = 0; i < n; ++i) {
    for (; !heap.empty();) {
      auto [r, l, a] = heap.top();
      if (r > i) break;
      heap.pop();
      heap_score -= a;
    }
    i64 this_score = 0;
    for (;; ++cur) {
      if (cur >= m) break;
      auto [l, r, a] = vt[cur];
      if (l > i) break;
      heap.emplace(r, l, a);
      this_score += a;
      heap_score += a;
    }
    dp[0][i + 1] = max(dp[0][i], dp[1][i]);
    dp[1][i+1] = max(dp[0][i] + heap_score,
    dp[1][i] + heap_score

    if (heap.empty()) {
      dp[i + 1] = dp[i];
    } else {
      dp[i + 1] = max(dp[i], dp[l] + heap_score);
    }
  }
  cout << dp[n] << endl;
}
