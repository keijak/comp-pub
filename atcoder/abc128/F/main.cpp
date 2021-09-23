#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in;
  const vector<i64> s = in(n);
  i64 ans = 0;
  const i64 hn = n / 2;
  for (i64 d = 1; d <= hn; ++d) {
    i64 acc = 0;
    std::bitset<100001> taken;
    for (i64 x = d, y = n - 1 - d;; x += d, y -= d) {
      if (x == y or x >= n - 1 - d or y <= d or taken[x] or taken[y]) break;
      taken.set(x, true);
      taken.set(y, true);
      acc += s[x] + s[y];
      chmax(ans, acc);
    }
  }
  cout << ans << "\n";
}
