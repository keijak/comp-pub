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

const i64 MOD = 1000000007LL;

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
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<int> flavor(N);
  for (auto& x : flavor) {
    cin >> x;
    x--;
  }

  // Two pointers.
  int head = 0, tail = 0;
  bitset<100000> used;
  vector<mint> dp(N);
  while (tail < N) {
    int f = flavor[tail];
    if (used[f]) break;
    used[f] = true;
    ++tail;
  }
  if (head < N) dp[head] += 1;
  if (tail < N) dp[tail] -= 1;
  while (head < N) {
    used[flavor[head]] = false;
    dp[head] += (head > 0) ? dp[head - 1] : 0;
    ++head;
    while (tail < N) {
      int f = flavor[tail];
      if (used[f]) break;
      used[f] = true;
      ++tail;
    }
    // Range Add.
    if (head < N) {
      dp[head] += dp[head - 1];
    }
    if (tail < N) {
      dp[tail] -= dp[head - 1];
    }
  }
  cout << dp[N - 1] << endl;
}