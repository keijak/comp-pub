#include <bits/stdc++.h>

#include <atcoder/math>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

using namespace std;

const vector<int> primes = {2,  3,  5,  7,  11, 13, 17, 19, 23, 29,
                            31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
const int M = primes.size();

i64 solve() {
  i64 A, B;
  cin >> A >> B;

  vector dp(M, vector(1 << M, optional<u64>{}));
  auto f = [&](auto &f, int i, unsigned sel) -> u64 {
    if (i == M) {
      return 1;
    }
    if (dp[i][sel].has_value()) {
      return dp[i][sel].value();
    }
    u64 mask = (1 << (i + 1)) - 1;

    u64 ans = f(f, i + 1, sel & ~mask);
    if (sel & (1 << i)) {
      dp[i][sel] = ans;
      return ans;
    }

    i64 p = primes[i];
    for (i64 x = B / p * p; x >= A; x -= p) {
      unsigned high = 0;
      bool ok = true;
      REP(j, M) {
        if (x % primes[j] == 0) {
          if (j < i) {
            ok = false;
            break;
          }
          if (sel & (1 << j)) {
            ok = false;
            break;
          }
          high |= 1 << j;
        }
      }
      if (not ok) continue;
      ans += f(f, i + 1, (sel | high) & ~mask);
    }
    dp[i][sel] = ans;
    return ans;
  };
  u64 a = f(f, 0, 0);
  for (i64 x = A; x <= B; ++x) {
    bool p = true;
    REP(i, M) {
      if (x % primes[i] == 0) {
        p = false;
        break;
      }
    }
    if (p) a <<= 1;
  }
  return a;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
