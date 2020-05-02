/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;



void solve(long long N, std::vector<long long> w, std::vector<long long> s, std::vector<long long> v){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N;
  scanf("%lld",&N);
  std::vector<long long> w(N);
  std::vector<long long> s(N);
  std::vector<long long> v(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&w[i]);
    scanf("%lld",&s[i]);
    scanf("%lld",&v[i]);
  }
  solve(N, std::move(w), std::move(s), std::move(v));
}
