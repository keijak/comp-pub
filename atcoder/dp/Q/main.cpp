#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

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

void solve(int N, std::vector<int> h, std::vector<long long> a) {
  SegTree<int64> dp(
      N + 1, [](int64 a, int64 b) { return max<int64>(a, b); }, 0LL);
  dp.update(h[0], a[0]);
  int64 res = a[0];
  for (int i = 1; i < N; ++i) {
    int ht = h[i];
    int64 bmax = dp.get(0, ht) + a[i];
    dp.update(ht, bmax);
    res = max(res, bmax);
  }
  cout << res << endl;
}

int main() {
  int N;
  cin >> N;
  std::vector<int> h(N);
  for (auto &&v : h) {
    cin >> v;
  }
  std::vector<long long> a(N);
  for (auto &&v : a) {
    cin >> v;
  }
  solve(N, std::move(h), std::move(a));
}
