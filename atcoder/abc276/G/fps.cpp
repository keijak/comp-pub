#define NDEBUG
#include <bits/stdc++.h>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

using namespace std;

namespace mints {

std::vector<Mint> vec(std::initializer_list<Mint> v) {
  static constexpr size_t kInitReserve = 1 << 20;
  std::vector<Mint> ret(v);
  ret.reserve(kInitReserve);
  return ret;
}

Mint inv(int n) {
  static std::vector<Mint> vals = vec({0, 1});
  for (int i = (int) vals.size(); i <= n; ++i) {
    const int q = Mint::mod() / i;
    const int r = Mint::mod() % i;
    vals.push_back(-vals[r] * q);
  }
  return vals[n];
}

Mint fact(int n) {
  static std::vector<Mint> vals = vec({1, 1});
  for (int i = (int) vals.size(); i <= n; ++i) {
    vals.push_back(vals.back() * i);
  }
  return vals[n];
}

Mint invfact(int n) {
  static std::vector<Mint> vals = vec({1, 1});
  for (int i = (int) vals.size(); i <= n; ++i) {
    vals.push_back(vals.back() * inv(i));
  }
  return vals[n];
}

// nCk
Mint binom(int n, int k) {
  if (k < 0 || k > n) return 0;
  return fact(n) * invfact(k) * invfact(n - k);
}

} // namespace mints
using mints::binom;

Mint solve(int N, int M) {
  if (M - N + 1 < 0) return 0;
  vector<Mint> f(M + 1);
  for (int i = 0; i < N; ++i) {
    int k = i + 2 * (N - 1 - i);
    if (k <= M) {
      f[k] += binom(N - 1, i);
    }
  }
  vector<Mint> g(M + 1);
  for (int i = 0; 3 * i <= M; ++i) {
    int k = 3 * i;
    g[k] += binom(N - 2 + i, i);
  }
  REP(d, 2) REP(i, M) g[i + 1] += g[i];
  Mint ans = 0;
  REP(i, M + 1) {
    ans += f[i] * g[M - i];
  }
  return ans;
}

int main() {
  int n, m;
  cin >> n >> m;
  cout << solve(n, m) << endl;
}
