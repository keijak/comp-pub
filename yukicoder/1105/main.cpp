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

vector<vector<mint>> matmul(const vector<vector<mint>>& a,
                            const vector<vector<mint>>& b) {
  int l = a[0].size();
  vector<vector<mint>> ret(l, vector<mint>(l, 0));
  for (int i = 0; i < l; ++i) {
    for (int j = 0; j < l; ++j) {
      for (int k = 0; k < l; ++k) {
        ret[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return ret;
}

vector<vector<mint>> matpow(const vector<vector<mint>>& m, i64 n) {
  int l = m[0].size();
  if (n == 0) {
    vector<vector<mint>> ret(m);
    for (int i = 0; i < l; ++i) {
      for (int j = 0; j < l; ++j) {
        ret[i][j] = (i == j);
      }
    }
    return ret;
  }
  if (n == 1) return m;
  if (n % 2) {
    return matmul(matpow(m, n - 1), m);
  } else {
    return matpow(matmul(m, m), n / 2);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, a0, b0, c0;
  cin >> N >> a0 >> b0 >> c0;
  vector<vector<mint>> A = {{1, -1, 0}, {0, 1, -1}, {-1, 0, 1}};
  auto X = matpow(A, N - 1);
  mint an = X[0][0] * a0 + X[0][1] * b0 + X[0][2] * c0;
  mint bn = X[1][0] * a0 + X[1][1] * b0 + X[1][2] * c0;
  mint cn = X[2][0] * a0 + X[2][1] * b0 + X[2][2] * c0;
  cout << an << " " << bn << " " << cn << '\n';
}
