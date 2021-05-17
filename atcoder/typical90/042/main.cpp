#include <bits/stdc++.h>

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

Mint solve() {
  int K;
  cin >> K;
  if (K % 9 != 0) return Mint(0);

  vector<optional<Mint>> memo(K + 1);
  memo[0] = 1;
  memo[1] = 1;
  auto f = [&](auto &f, int x) -> Mint {
    if (memo[x]) return *memo[x];
    Mint res = 0;
    for (int i = 1; i <= 9; ++i) {
      if (x - i < 0) break;
      res += f(f, x - i);
    }
    memo[x] = res;
    return res;
  };
  return f(f, K);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
