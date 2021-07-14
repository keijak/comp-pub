#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

template <class Monoid>
struct SegTree {
  using Func = function<Monoid(Monoid, Monoid)>;
  const Func F;
  const Monoid UNITY;
  int SIZE_R;
  vector<Monoid> dat;

  SegTree(int n, const Func f, const Monoid &unity) : F(f), UNITY(unity) {
    init(n);
  }
  void init(int n) {
    SIZE_R = 1;
    while (SIZE_R < n) SIZE_R *= 2;
    dat.assign(SIZE_R * 2, UNITY);
  }

  /* set, a is 0-indexed */
  void set(int a, const Monoid &v) { dat[a + SIZE_R] = v; }
  void build() {
    for (int k = SIZE_R - 1; k > 0; --k) dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
  }

  /* update a, a is 0-indexed */
  void update(int a, const Monoid &v) {
    int k = a + SIZE_R;
    dat[k] = v;
    while (k >>= 1) dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
  }

  /* get [a, b), a and b are 0-indexed */
  Monoid get(int a, int b) {
    Monoid vleft = UNITY, vright = UNITY;
    for (int left = a + SIZE_R, right = b + SIZE_R; left < right;
         left >>= 1, right >>= 1) {
      if (left & 1) vleft = F(vleft, dat[left++]);
      if (right & 1) vright = F(dat[--right], vright);
    }
    return F(vleft, vright);
  }
  inline Monoid operator[](int a) { return dat[a + SIZE_R]; }

  /* debug */
  void print() {
    for (int i = 0; i < SIZE_R; ++i) {
      cout << (*this)[i];
      if (i != SIZE_R - 1) cout << ",";
    }
    cout << endl;
  }
};

const i64 INF = 1LL << 40;
const int K = 200001;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<i64> rate(N);
  vector<int> bel(N);
  vector<set<pair<i64, int>>> kin(K);
  REP(i, N) {
    int a, b;
    cin >> a >> b;
    b--;
    rate[i] = a;
    bel[i] = b;
    kin[b].emplace(a, i);
  }

  SegTree<i64> st(
      K, [](i64 x, i64 y) -> i64 { return min(x, y); }, INF);
  REP(i, K) {
    if (!kin[i].empty()) {
      auto [maxrate, j] = *kin[i].rbegin();
      st.set(i, maxrate);
    }
  }
  st.build();

  auto maxrate = [&](const set<pair<i64, int>> &s) -> i64 {
    if (s.empty()) return INF;
    return s.rbegin()->first;
  };

  REP(i, Q) {
    int c, d;
    cin >> c >> d;
    c--;
    d--;
    int belp = bel[c];
    bel[c] = d;
    auto p = make_pair(rate[c], c);
    kin[belp].erase(p);
    kin[d].insert(p);
    st.update(belp, maxrate(kin[belp]));
    st.update(d, maxrate(kin[d]));

    cout << st.get(0, K) << '\n';
  }
}
