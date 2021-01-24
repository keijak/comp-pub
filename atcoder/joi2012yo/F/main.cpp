#include <bits/stdc++.h>

#include <atcoder/modint>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using Mint = atcoder::static_modint<10000>;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

void decr(string &x) {
  int n = ssize(x);
  for (int i = n - 1; i >= 0; --i) {
    if (x[i] != '0') {
      x[i]--;
      for (int j = i + 1; j < n; ++j) {
        x[j] = '9';
      }
      auto ite = x.end();
      --ite;
      auto it = x.begin();
      while (it != ite and *it == '0') ++it;
      if (it != x.begin()) x.erase(x.begin(), it);
      return;
    }
  }
  assert(false);
}

Mint zigzag(const string &s, int m) {
  int n = ssize(s);
  vector<int> b(n, 0);
  {
    int x = 1;
    for (int i = n - 1; i >= 0; --i) {
      b[i] = x;
      x *= 10;
      x %= m;
    }
  }

  // p: 0=first, 1=decreased, 2=increased
  vector dp(2, vector(10, vector(m, vector(2, vector(3, Mint(0))))));
  {
    int d0 = s[0] - '0';
    REP(nd, 10) {
      if (nd > d0) continue;
      int j = (b[0] * nd) % m;
      bool smaller = nd < d0;
      dp[0][nd][j][smaller][0] = 1;
    }
  }

  REP(i, 1, n) {
    int ds = s[i] - '0';
    REP(pd, 10) REP(j, m) REP(smaller, 2) REP(p, 3) REP(nd, 10) {
      if (not smaller and nd > ds) continue;
      bool nx_smaller = smaller;
      if (nd < ds) nx_smaller = true;
      int nx_j = (j + b[i] * nd) % m;
      int nx_p = p;
      if (p == 1) {
        if (nd <= pd) continue;
        nx_p = 2;
      } else if (p == 2) {
        if (nd >= pd) continue;
        nx_p = 1;
      } else if (pd == 0) {
        nx_p = 0;
      } else if (nd > pd) {
        nx_p = 2;
      } else if (nd < pd) {
        nx_p = 1;
      } else {
        continue;
      }
      dp[1][nd][nx_j][nx_smaller][nx_p] += dp[0][pd][j][smaller][p];
    }
    swap(dp[0], dp[1]);
    dp[1].assign(10, vector(m, vector(2, vector(3, Mint(0)))));
  }

  Mint res = 0;
  REP(d, 10) REP(smaller, 2) REP(p, 3) { res += dp[0][d][0][smaller][p]; }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  string a, b;
  int m;
  cin >> a >> b >> m;
  Mint zb = zigzag(b, m);
  decr(a);
  Mint za = zigzag(a, m);
  cout << zb - za << "\n";
}
