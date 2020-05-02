#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(long long N, long long M, long long Q, std::vector<long long> a,
           std::vector<long long> b, std::vector<long long> c,
           std::vector<long long> d) {
  int64 maxp = 0;
  vector<int64> seq(N, -1);
  function<void(int, int)> search = [&](int i, int val) {
    if (i == N) {
      int64 dsum = 0;
      for (int j = 0; j < Q; ++j) {
        int64 aj = a[j] - 1;
        int64 bj = b[j] - 1;
        if (seq[bj] - seq[aj] == c[j]) {
          dsum += d[j];
        }
      }
      if (maxp < dsum) {
        maxp = dsum;
      }
      return;
    }
    seq[i] = val;
    for (int j = val; j <= min<int>(10, M); ++j) {
      search(i + 1, j);
    }
  };
  search(0, 1);
  cout << maxp << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld", &N);
  long long M;
  scanf("%lld", &M);
  long long Q;
  scanf("%lld", &Q);
  std::vector<long long> a(Q);
  std::vector<long long> b(Q);
  std::vector<long long> c(Q);
  std::vector<long long> d(Q);
  for (int i = 0; i < Q; i++) {
    scanf("%lld", &a[i]);
    scanf("%lld", &b[i]);
    scanf("%lld", &c[i]);
    scanf("%lld", &d[i]);
  }
  solve(N, M, Q, std::move(a), std::move(b), std::move(c), std::move(d));
}
