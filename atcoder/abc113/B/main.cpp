#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;


void solve(long long N, long long T, long long A, std::vector<long long> H){

}

int main() {
  long long N;
  scanf("%lld",&N);
  long long T;
  scanf("%lld",&T);
  long long A;
  scanf("%lld",&A);
  std::vector<long long> H(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&H[i]);
  }
  solve(N, T, A, std::move(H));
}
