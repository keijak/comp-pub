#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;


void solve(long long N, long long M, std::vector<long long> H, std::vector<long long> A, std::vector<long long> B){

}

int main() {
  long long N;
  scanf("%lld",&N);
  long long M;
  scanf("%lld",&M);
  std::vector<long long> H(N);
  for(int i = 0 ; i < N ; i++){
    scanf("%lld",&H[i]);
  }
  std::vector<long long> A(M);
  std::vector<long long> B(M);
  for(int i = 0 ; i < M ; i++){
    scanf("%lld",&A[i]);
    scanf("%lld",&B[i]);
  }
  solve(N, M, std::move(H), std::move(A), std::move(B));
}
