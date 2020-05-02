/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;


const long long MOD = 1000000007;

void solve(long long H, long long W, long long N, std::vector<long long> r, std::vector<long long> c){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long H;
  scanf("%lld",&H);
  long long W;
  scanf("%lld",&W);
  long long N;
  scanf("%lld",&N);
  std::vector<long long> r(N);
  std::vector<long long> c(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&r[i]);
    scanf("%lld",&c[i]);
  }
  solve(H, W, N, std::move(r), std::move(c));
}
