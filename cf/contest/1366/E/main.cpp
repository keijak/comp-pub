#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif
// auto mod int

const i64 MOD = 998244353LL;

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

i64 INF = 1LL << 40;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  vector<i64> B(M);
  for (auto& x : B) cin >> x;

  mint ans = 1;
  int j = 0;
  for (int i = 0; i < M; ++i) {
    i64 cur_min = B[i];
    i64 next_min = (i < M - 1) ? B[i + 1] : -1LL;
    int start = -1;
    bool min_found = false;
    while (j < N) {
      DEBUG(start, j, A[j], cur_min);
      if (A[j] == cur_min) {
        min_found = true;
        start = j;
      } else if (A[j] < cur_min) {
        break;
      }
      if (A[j] < next_min) {
        start = j;
      }
      j++;
    }
    _debug("choice");
    DEBUG(i, min_found, j - start + 1);
    if (!min_found) {
      ans = 0;
      break;
    }
    ans *= j - start + 1;
  }
  cout << ans.x << endl;
}
