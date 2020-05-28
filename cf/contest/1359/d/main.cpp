#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

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

const i64 INF = 1LL << 60;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x : a) cin >> x;

  SegTree<i64> ls(
      n + 1, [](i64 x, i64 y) -> i64 { return max(x, y); }, -INF);
  SegTree<i64> rs(
      n + 1, [](i64 x, i64 y) -> i64 { return max(x, y); }, -INF);
  ls.set(0, 0);
  rs.set(n, 0);
  vector<i64> la(n + 1), ra(n + 1), lm(n + 1), rm(n + 1);
  REP(i, n) {
    la[i + 1] = la[i] + a[i];
    ls.set(i + 1, la[i + 1]);
    int j = n - 1 - i;
    ra[j] = ra[j + 1] + a[j];
    rs.set(j, ra[j]);
  }
  ls.build();
  rs.build();

  vector<vector<int>> rindex(n + 1, vector<int>(61, -1));
  for (int j = n - 1; j >= 0; --j) {
    int x = a[j];
    REP(v, 61) {
      int y = v - 30;
      if (x == y) {
        rindex[j][v] = j;
      } else {
        rindex[j][v] = rindex[j + 1][v];
      }
    }
  }

  vector<int> lindex(61, -1);

  auto find_lmax = [&](int x, int i) -> int {
    int lmax = -1;
    for (int y = x + 1; y <= 30; ++y) {
      int j = lindex[y + 30];
      if (j >= 0) {
        lmax = max(lmax, j);
      }
    }
    return lmax;
  };
  auto find_rmin = [&](int x, int i) -> int {
    int rmin = n;
    for (int y = x + 1; y <= 30; ++y) {
      int j = rindex[i + 1][y + 30];
      if (j >= 0) {
        rmin = min(rmin, j);
      }
    }
    return rmin;
  };

  i64 ans = 0;
  REP(i, n) {
    const int x = a[i];
    int l = find_lmax(x, i);
    int r = find_rmin(x, i);
    i64 lsum = rs.get(l + 1, i + 1) - ra[i];
    i64 rsum = ls.get(i + 1, r + 1) - la[i + 1];
    if (ans < lsum + rsum) {
      ans = lsum + rsum;
    }
    lindex[x + 30] = i;
  }
  cout << ans << endl;
}
