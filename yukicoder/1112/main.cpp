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

// auto mod int

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

using MatrixRow = vector<mint>;
using Matrix = vector<MatrixRow>;

Matrix matmul(const Matrix& a, const Matrix& b) {
  int n = a.size();
  assert(int(a[0].size()) == n);
  assert(int(b.size()) == n);
  assert(int(b[0].size()) == n);

  Matrix ret(n, MatrixRow(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      for (int k = 0; k < n; ++k) {
        ret[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return ret;
}

Matrix matpow(const Matrix& a, long long k) {
  int n = a.size();
  assert(int(a[0].size()) == n);

  if (k == 0) {
    Matrix eye(n, MatrixRow(n));
    for (int i = 0; i < n; ++i) {
      eye[i][i] = 1;
    }
    return eye;
  }

  if (k == 1) {
    return a;
  }

  if (k & 1) {
    return matmul(matpow(a, k - 1), a);
  } else {
    return matpow(matmul(a, a), k / 2);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int K, M;
  cin >> K >> M;
  i64 N;
  cin >> N;
  int kk = K * K;
  Matrix mat(kk, MatrixRow(kk));
  MatrixRow init(kk);
  set<int> out;
  REP(i, M) {
    int p, q, r;
    cin >> p >> q >> r;
    p--;
    q--;
    r--;
    int from = p * K + q;
    int to = q * K + r;
    mat[to][from] = 1;
    if (p == 0) init[to] += 1;
    if (r == 0) out.insert(to);
  }

  Matrix mn = matpow(mat, N - 3);

  mint ans = 0;
  for (auto x : out) {
    REP(j, kk) { ans += mn[x][j] * init[j]; }
  }
  cout << ans << endl;
}
