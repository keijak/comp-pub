#include <bits/stdc++.h>

#include <atcoder/dsu>
#include <atcoder/modint>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using namespace std;
using Mint = atcoder::modint998244353;

Mint factorial(int x) {
  static std::vector<Mint> facts = {1, 1, 2};
  facts.reserve(51);
  while ((int)facts.size() <= x) {
    facts.push_back(facts.back() * facts.size());
  }
  return facts[x];
}

int solve() {
  int n, K;
  cin >> n >> K;
  auto a = vector(n, vector(n, 0));
  REP(i, n) REP(j, n) cin >> a[i][j];

  atcoder::dsu col_dsu(n), row_dsu(n);

  REP(i, n) REP(j, i) {
    REP(k, n) {
      if (a[k][i] + a[k][j] > K) goto invalid1;
    }
    col_dsu.merge(j, i);
  invalid1:
    continue;
  }

  REP(i, n) REP(j, i) {
    REP(k, n) {
      if (a[i][k] + a[j][k] > K) goto invalid2;
    }
    row_dsu.merge(j, i);
  invalid2:
    continue;
  }

  Mint cx = 1, rx = 1;
  for (auto c : col_dsu.groups()) cx *= factorial(ssize(c));
  for (auto r : row_dsu.groups()) rx *= factorial(ssize(r));
  return (cx * rx).val();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << endl;
}