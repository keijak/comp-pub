#include <bits/stdc++.h>
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

i64 MOD = 1'000'000'007;

struct mint {
  long long x;
  mint(long long x = 0) : x((x % MOD + MOD) % MOD) {}
  mint operator-() const { return mint(-x); }
  mint& operator+=(const mint a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint& operator-=(const mint a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint& operator*=(const mint a) {
    (x *= a.x) %= MOD;
    return *this;
  }
  mint operator+(const mint a) const { return mint(*this) += a; }
  mint operator-(const mint a) const { return mint(*this) -= a; }
  mint operator*(const mint a) const { return mint(*this) *= a; }
  mint pow(long long t) const {
    if (!t) return 1;
    mint a = pow(t >> 1);
    a *= a;
    if (t & 1) a *= *this;
    return a;
  }

  // for prime MOD
  mint inv() const { return pow(MOD - 2); }
  mint& operator/=(const mint a) { return *this *= a.inv(); }
  mint operator/(const mint a) const { return mint(*this) /= a; }
};
istream& operator>>(istream& is, mint& a) { return is >> a.x; }
ostream& operator<<(ostream& os, const mint& a) { return os << a.x; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n >> MOD;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    g[x].push_back(y);
    g[y].push_back(x);
  }

  vector<mint> sub_counts(n);  // subtree sizes of black nodes.
  auto calc_sub = [&](auto rec, int v, int p) -> mint {
    mint cnt = 1;
    for (auto u : g[v]) {
      if (u != p) {
        cnt *= rec(rec, u, v) + 1;
      }
    }
    return (sub_counts[v] = cnt);
  };
  calc_sub(calc_sub, 0, -1);

  vector<mint> all_counts(n);
  auto calc_all = [&](auto rec, int v, int p, mint pcount) -> void {
    int sz = g[v].size();
    all_counts[v] = sub_counts[v] * (pcount + 1);
    vector<mint> lcum(sz + 1, 1), rcum(sz + 1, 1);
    REP(i, sz) {
      int u = g[v][i];
      lcum[i + 1] = lcum[i] * (u == p ? mint(1) : (sub_counts[u] + 1));
    }
    for (int i = sz - 1; i >= 0; --i) {
      int u = g[v][i];
      rcum[i] = rcum[i + 1] * (u == p ? mint(1) : (sub_counts[u] + 1));
    }
    REP(i, sz) {
      int u = g[v][i];
      if (u != p) {
        rec(rec, u, v, (pcount + 1) * lcum[i] * rcum[i + 1]);
      }
    }
  };
  calc_all(calc_all, 0, -1, mint(0));

  REP(i, n) { cout << all_counts[i] << '\n'; }
}
