#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;

template <typename T>
using V = vector<T>;

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

#define chmax(x, y) x = max(x, y)
const i64 INF = 1LL << 40;

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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, K, M;
  cin >> N >> K >> M;
  V<i64> A(N);
  for (auto &x : A) cin >> x;
  V<i64> acum(N + 1);
  REP(i, N) { acum[i + 1] = acum[i] + A[i]; }
  V<i64> dp(N + 1, -INF);
  dp[0] = 0;
  auto mx = [](i64 x, i64 y) { return max(x, y); };
  SegTree<i64> st1(N + 1, mx, -INF), st2(N + 1, mx, -INF);
  st1.update(0, 0);
  st2.update(0, 0);
  for (int k = 1; k <= K; ++k) {
    for (int i = 1; i <= N; ++i) {
      st1.update(i, dp[i] - acum[i]);
      st2.update(i, dp[i] + acum[i]);
    }
    for (int i = 1; i <= N; ++i) {
      int start = max<int>(i - M, 0);
      dp[i] = max(st1.fold(start, i) + acum[i], st2.fold(start, i) - acum[i]);
    }
  }
  cout << dp[N] << endl;
}
