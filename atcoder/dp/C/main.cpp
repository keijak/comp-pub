/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;



void solve(long long N, std::vector<long long> a, std::vector<long long> b, std::vector<long long> c){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  std::vector<long long> a(N);
  std::vector<long long> b(N);
  std::vector<long long> c(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&a[i]);
    scanf("%lld",&b[i]);
    scanf("%lld",&c[i]);
  }
  solve(N, std::move(a), std::move(b), std::move(c));
}
