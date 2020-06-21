#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

// auto mod int

const i64 MOD = 998244353;

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
  string S;
  cin >> S;
  int K;
  cin >> K;
  const int N = S.size();
  deque<char> q(S.begin(), S.end());
  set<deque<char>> qset;
  qset.insert(q);

  auto rotate = [&](int i, int j) -> void {
    assert(q[i] == '0');
    assert(q[j] == '1');
    for (int r = j; r > i; --r) {
      q[r] = q[r - 1];
    }
    q[i] = '1';
  };
  auto rotate_back = [&](int i, int j) -> void {
    assert(q[i] == '1');
    for (int r = i; r < j; ++r) {
      q[r] = q[r + 1];
    }
    q[j] = '1';
  };

  set<pair<int, int>> seen;
  auto solve = [&](auto rec, int i, int k) -> void {
    if (i == N || k == 0) return;
    if (seen.count(make_pair(i, k))) return;
    char c = q[i];
    rec(rec, i + 1, k);
    if (c == '1') {
      return;
    }
    int j = i + 1;
    for (; j < N; ++j) {
      if (q[j] == '1') {
        rotate(i, j);
        qset.insert(q);
        rec(rec, i + 1, k - 1);
        rotate_back(i, j);
      }
    }
    seen.emplace(i, k);
  };
  solve(solve, 0, K);
  cout << qset.size() << '\n';
}