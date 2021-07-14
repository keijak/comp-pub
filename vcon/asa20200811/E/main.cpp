#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string pat;
  cin >> pat;
  int n = pat.size();
  unsigned bpat = 0;
  unsigned full = 0;
  REP(i, n) {
    if (pat[i] == 'o') {
      bpat |= 1 << i;
    }
    full |= 1 << i;
  }
  unsigned wpat = bpat | (bpat << n);
  const int INF = n + 100;

  auto solve = [&](auto self, unsigned cur, int num, int shift) -> int {
    if ((cur & full) == full) {
      return num;
    }
    int res = INF;
    for (int s = shift + 1; s < n; ++s) {
      res = min(res, self(self, cur | (wpat >> s), num + 1, s));
    }
    return res;
  };

  cout << solve(solve, bpat, 1, 0) << endl;
}
