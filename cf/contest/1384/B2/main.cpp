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

i64 MOD;
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

bool solve() {
  i64 n, k, l;
  cin >> n >> k >> l;
  i64 k2 = 2 * k;
  MOD = k2;
  vector<i64> d(n);
  REP(i, n) cin >> d[i];
  vector<int> safes;
  REP(i, n) {
    if (d[i] > l) {
      return false;
    }
    if (d[i] + k <= l) {
      safes.push_back(i + 1);
    }
  }
  safes.push_back(n + 1);

  int cur_x = 0;
  REP(i, safes.size()) {
    map<i64, int> cum;
    int next_x = safes[i];
    DEBUG(next_x);
    i64 seg_size = 0;
    for (int j = cur_x + 1; j < next_x; ++j) {
      DEBUG(j - 1);
      ++seg_size;
      i64 h = l - d[j - 1];
      assert(h < k);  // unsafe
      assert(h >= 0);
      mint dist = mint(j) - mint(cur_x);
      mint tstart = -mint(h) - dist;
      mint tend = mint(h) - dist + 1;
      DEBUG(h, dist, tstart, tend);
      assert(tstart.x != tend.x);
      if (tstart.x < tend.x) {
        cum[tstart.x] += 1;
        cum[tend.x] -= 1;
      } else {
        cum[tstart.x] += 1;
        cum[k2] -= 1;
        cum[0] += 1;
        cum[tend.x] -= 1;
      }
    }
    cur_x = next_x;

    i64 s = 0;
    bool ok = seg_size == 0;
    for (auto [x, delta] : cum) {
      s += delta;
      if (s == seg_size) {
        ok = true;
        break;
      }
    }
    if (!ok) return false;
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    DEBUG(t);
    cout << (solve() ? "Yes\n" : "No\n");
  }
}
