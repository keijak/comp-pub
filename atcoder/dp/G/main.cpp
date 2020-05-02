/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;



void solve(long long N, long long M, std::vector<long long> x, std::vector<long long> y){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  long long M;
  scanf("%lld",&M);
  std::vector<long long> x(M);
  std::vector<long long> y(M);
  for(int i = 0 ; i < M ; i++){
    scanf("%lld",&x[i]);
    scanf("%lld",&y[i]);
  }
  solve(N, M, std::move(x), std::move(y));
}
