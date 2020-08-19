#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<tuple<int, int, i64>> inputs(M);
  REP(i, M) {
    int l, r;
    i64 c;
    cin >> l >> r >> c;
    inputs[i] = {r, l, c};
  }
  sort(inputs.begin(), inputs.end());

  const i64 INF = 1e18;
  SegTree<i64> st(
      N + 10, [](i64 x, i64 y) { return min(x, y); }, INF);
  st.update(1, 0LL);

  for (auto [r, l, c] : inputs) {
    i64 val = st.fold(l, r + 1);
    if (val < INF) {
      i64 y = st[r];
      if (y > val + c) {
        st.update(r, val + c);
      }
    }
  }

  i64 ans = st[N];
  cout << (ans < INF ? ans : -1LL) << endl;
}
