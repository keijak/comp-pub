#define ENABLE_DEBUG 0
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

i64 solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  for (auto& x : a) cin >> x;
  i64 ans = 0;
  for (int i = 0; i < n; i += 2) {
    ans += a[i];
  }

  auto maxgain = [&](const vector<i64>& d) -> i64 {
    i64 ans = 0;
    i64 sum = 0;
    REP(i, d.size()) {
      sum = max(sum + d[i], d[i]);
      ans = max(ans, sum);
    }
    return ans;
  };

  vector<i64> b;
  for (int i = 0; i + 1 < n; i += 2) {
    b.push_back(a[i + 1] - a[i]);
  }
  i64 gain1 = maxgain(b);

  vector<i64> b2;
  for (int i = 2; i < n; i += 2) {
    b2.push_back(a[i - 1] - a[i]);
  }
  i64 gain2 = maxgain(b2);
  ans += max(gain1, gain2);
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << solve() << '\n';
  }
}
