/* Time: $%Y%$-$%M%$-$%D%$ $%h%$:$%m%$:$%s%$ */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;


const long long MOD = 1000000007;

void solve(long long H, long long W, std::vector<std::vector<std::string>> a){

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long H;
  scanf("%lld",&H);
  long long W;
  scanf("%lld",&W);
  std::vector<std::vector<std::string>> a(H, std::vector<std::string>(1));
  for(int i = 0 ; i < H ; i++){
    for(int j = 0 ; j < 1 ; j++){
      std::cin >> a[i][j];
    }
  }
  solve(H, W, std::move(a));
}
