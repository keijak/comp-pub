#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

class Solution {
 public:
  long long appealSum(string s) {
    const int n = s.size();
    vector<int> last(26, -1);
    Int acc = 0;
    Int ans = 0;
    for (int r = 1; r <= n; ++r) {
      char ch = s[r - 1];
      int li = last[ch - 'a'];
      acc += r - (li + 1);
      ans += acc;
      last[ch - 'a'] = r - 1;
    }
    return ans;
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  Solution sol;
  auto ans = sol.appealSum("abbca");
  DUMP(ans);
  exit_();
}
