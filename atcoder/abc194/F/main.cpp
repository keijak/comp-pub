#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

// C[n][k] = nCk
// Cretates a combination matrix using Pascal's triangle.
// Time and space complexity: Θ(n^2)
// Useful to avoid factorials causing integer overflow.
template <class T = Mint>
std::vector<std::vector<T>> comb_table(int n) {
  std::vector<std::vector<T>> C(n + 1, std::vector<T>(n + 1, 0));
  for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C;
}

Mint solve() {
  string N;
  int K;
  cin >> N >> K;
  const int m = N.size();
  array<bool, 16> used = {};
  int used_count = 0;
  Mint ans = 0;
  auto C = comb_table(16);

  vector dp(m + 1, vector(17, optional<Mint>{}));

  auto f = [&](auto &f, int l, int p) -> Mint {
    int q = K - p;
    if (l < 0) return 0;
    if (q > l) return 0;
    if (l == 0) return 1;
    auto &cached = dp[l][p];
    if (cached) return *cached;

    Mint res = 0;
    if (p > 0) {
      res += f(f, l - 1, p) * p;  // already used
    }
    if (p < K) {
      res += f(f, l - 1, p + 1) * q;  // new digit
    }
    cached = res;
    return res;
  };

  REP(i, m) {
    char hc = N[i];
    int hd = 0;
    if ('0' <= hc and hc <= '9') {
      hd = hc - '0';
    } else {
      hd = 10 + (hc - 'A');
    }

    int u = 0, v = 0;
    for (int d = 0; d < hd; ++d) {
      if (d == 0 and i == 0) continue;
      if (used[d]) {
        ++u;
      } else {
        ++v;
      }
    }
    if (u > 0) {
      int p = used_count;
      if (p <= K) {
        ans += f(f, m - i - 1, p) * C[16 - p][K - p] * u;
      }
    }
    if (v > 0) {
      int p = used_count + 1;
      if (p <= K) {
        ans += f(f, m - i - 1, p) * C[16 - p][K - p] * v;
      }
    }

    if (not used[hd]) {
      used[hd] = true;
      used_count++;
      if (used_count > K) break;
    }
  }

  if (used_count == K) {
    ans++;  // include N.
  }

  Mint r0 = 0;
  REP(i, 1, m) { r0 += f(f, m - i - 1, 1) * 15 * C[15][K - 1]; }
  ans += r0;

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
