#include <iostream>
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
using namespace std;
int main() {
  int N, K;
  cin >> N >> K;
  auto ans = Mint(N) * (N - 1) * (K + 1) * (2 * K + 1) / (12 * K);
  cout << ans.val() << endl;
}
