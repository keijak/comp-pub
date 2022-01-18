#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

template<std::uint_fast64_t Modulus>
class modint {
  using u64 = std::uint_fast64_t;

 public:
  u64 a;

  modint(const u64 x = 0) noexcept: a(x % Modulus) {}
  u64 &value() noexcept { return a; }
  const u64 &value() const noexcept { return a; }
  modint operator+(const modint rhs) const noexcept {
    return modint(*this) += rhs;
  }
  modint operator-(const modint rhs) const noexcept {
    return modint(*this) -= rhs;
  }
  modint operator*(const modint rhs) const noexcept {
    return modint(*this) *= rhs;
  }
  modint operator/(const modint rhs) const noexcept {
    return modint(*this) /= rhs;
  }
  modint &operator+=(const modint rhs) noexcept {
    a += rhs.a;
    if (a >= Modulus) {
      a -= Modulus;
    }
    return *this;
  }
  modint &operator-=(const modint rhs) noexcept {
    if (a < rhs.a) {
      a += Modulus;
    }
    a -= rhs.a;
    return *this;
  }
  modint &operator*=(const modint rhs) noexcept {
    a = a * rhs.a % Modulus;
    return *this;
  }
  modint &operator/=(modint rhs) noexcept {
    u64 exp = Modulus - 2;
    while (exp) {
      if (exp % 2) {
        *this *= rhs;
      }
      rhs *= rhs;
      exp /= 2;
    }
    return *this;
  }
};

using Mint = modint<1000000007>;

int main() {
  int n;
  int k;
  cin >> n >> k;
  auto dp = vector<vector<Mint>>(n + 1, vector<Mint>(k + 2, 0));
  dp[1][0] = 1;
  for (int i = 2; i <= n; ++i) {
    for (int j = 0; j <= k; ++j) {
      Mint cur = dp[i - 1][j];
      if (cur.value() == 0) continue;
      dp[i][j] += cur;
      int r = min(j + i, k + 1);
      dp[i][r] -= cur;
    }
    for (int j = 1; j <= k; ++j) {
      dp[i][j] += dp[i][j - 1];
    }
  }
  cout << dp[n][k].value() << endl;
  return 0;
}
