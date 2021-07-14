#include <bits/stdc++.h>

#include <atcoder/modint>

#define REP(i, N) for (int i = 0, REP_N_ = (N); i < REP_N_; ++i)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

Mint solve() {
  string S;
  int K;
  cin >> S >> K;
  const int N = ssize(S);
  vector<int> runs;
  {
    int i = 0;
    while (i < N) {
      int j = 0;
      while (i < N and S[i] == '1') {
        ++i, ++j;
      }
      runs.push_back(j);
      ++i;
    }
  }
  const int M = ssize(runs);

  // dp[i][d][p]:
  // - processed i buckets from the back
  // - decremented d times <= L
  // - incremented p times <= d
  const int L = min(N, K);
  auto dp = vector(M + 1, vector(L + 1, vector(L + 1, Mint(0))));
  dp[0][0][0] = 1;
  REP(i, M) {
    int ri = M - 1 - i;
    int h = runs[ri];
    for (int d = 0; d <= L; ++d) {
      for (int p = 0; p <= d; ++p) {
        dp[i + 1][d][p] += dp[i][d][p];
        for (int j = 1; p + j <= d; ++j) {
          dp[i + 1][d][p + j] += dp[i][d][p];
        }
        for (int j = 1; j <= h; ++j) {
          if (d + j > L) break;
          dp[i + 1][d + j][p] += dp[i][d][p];
        }
      }
    }
  }
  Mint ans = 0;
  REP(i, L + 1) { ans += dp[M][i][i]; }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << endl;
}
