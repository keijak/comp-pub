/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;



void solve(long long N, long long M, std::vector<long long> l, std::vector<long long> r, std::vector<long long> a){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  long long M;
  scanf("%lld",&M);
  std::vector<long long> l(M);
  std::vector<long long> r(M);
  std::vector<long long> a(M);
  for(int i = 0 ; i < M ; i++){
    scanf("%lld",&l[i]);
    scanf("%lld",&r[i]);
    scanf("%lld",&a[i]);
  }
  solve(N, M, std::move(l), std::move(r), std::move(a));
}
