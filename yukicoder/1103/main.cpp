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

const i64 MOD = 1'000'000'007;

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
  int N;
  cin >> N;
  vector<vector<int>> child(N);
  vector<int> parent(N, -1);
  REP(i, N - 1) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    child[a].push_back(b);
    assert(parent[b] == -1);
    parent[b] = a;
  }
  int root = -1;
  REP(i, N) {
    if (parent[i] == -1) {
      assert(root == -1);
      root = i;
    }
  }

  vector<int> subsize(N);
  auto calc_subsize = [&](auto rec, int v) -> int {
    int sz = 1;
    for (int u : child[v]) {
      sz += rec(rec, u);
    }
    subsize[v] = sz;
    return sz;
  };
  calc_subsize(calc_subsize, root);
  REP(i, N) { DEBUG(i, subsize[i]); }

  mint ans = 0;
  auto calc_psum = [&](auto rec, int v, mint plen) -> void {
    for (int u : child[v]) {
      mint x = plen * subsize[u];
      ans += x;
      DEBUG(v, u, plen, subsize[u], x, ans);
      rec(rec, u, plen + 1);
    }
  };
  calc_psum(calc_psum, root, 1);

  cout << ans << '\n';
}