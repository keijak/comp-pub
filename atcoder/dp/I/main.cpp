/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;



void solve(long long N, std::vector<long double> p){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  std::vector<long double> p(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%Lf",&p[i]);
  }
  solve(N, std::move(p));
}
