#define ENABLE_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
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
  vector<int> T(N);
  for (auto& x : T) cin >> x;
  vector<int> A(N);
  for (auto& x : A) cin >> x;

  auto solve = [&]() -> mint {
    if (T.back() != A.front()) return 0;
    const int hmax = T.back();
    int prev = hmax;
    mint ans = 1;
    int ti = N - 1;
    for (int i = N - 1; i >= 0; --i) {
      if (T[i] == hmax) {
        ti = i;
        continue;
      }
      if (T[i] > prev) return 0;
      if (T[i] == prev) {
        ans *= prev;
      }
      prev = T[i];
    }
    int ai = 0;
    prev = hmax;
    REP(i, N) {
      if (A[i] == hmax) {
        ai = i;
        continue;
      }
      if (A[i] > prev) return 0;
      if (A[i] == prev) {
        ans *= prev;
      }
      prev = A[i];
    }
    if (ai < ti) return 0;
    if (ai - ti - 1 > 0) {
      ans *= mint(hmax).pow(ai - ti - 1);
    }
    return ans;
  };

  cout << solve() << '\n';
}