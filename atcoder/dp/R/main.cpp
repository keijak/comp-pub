/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;


const long long MOD = 1000000007;

void solve(long long N, long long K, std::vector<std::vector<long long>> a){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  long long K;
  scanf("%lld",&K);
  std::vector<std::vector<long long>> a(N, std::vector<long long>(N));
  for(int i = 0 ; i < N ; i++){
    for(int j = 0 ; j < N ; j++){
      scanf("%lld",&a[i][j]);
    }
  }
  solve(N, K, std::move(a));
}
