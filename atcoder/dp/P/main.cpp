/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;


const long long MOD = 1000000007;

void solve(long long N, std::vector<long long> x, std::vector<long long> y){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  std::vector<long long> x(N-1);
  std::vector<long long> y(N-1);
  for(int i = 0 ; i < N-1 ; i++){
    scanf("%lld",&x[i]);
    scanf("%lld",&y[i]);
  }
  solve(N, std::move(x), std::move(y));
}
