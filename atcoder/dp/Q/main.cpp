/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;



void solve(long long N, std::vector<long long> h, std::vector<long long> a){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  std::vector<long long> h(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&h[i]);
  }
  std::vector<long long> a(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&a[i]);
  }
  solve(N, std::move(h), std::move(a));
}
