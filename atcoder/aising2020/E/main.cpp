#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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

i64 solve() {
  i64 N;
  cin >> N;
  vector<tuple<i64, i64, i64, i64>> lefty;
  vector<tuple<i64, i64, i64, i64>> righty;
  REP(i, N) {
    i64 k, l, r;
    cin >> k >> l >> r;
    if (l >= r) {
      lefty.emplace_back(l - r, k, l, r);
    } else {
      righty.emplace_back(r - l, k, l, r);
    }
  }
  std::sort(lefty.rbegin(), lefty.rend());
  std::sort(righty.rbegin(), righty.rend());

  std::set<int> available;
  REP(i, N) available.insert(i);
  available.insert(-1);  // sentinel
  available.insert(N);   // sentinel

  i64 ans = 0;
  for (auto [gain, k, l, r] : lefty) {
    int j = min<int>(k - 1, lefty.size() - 1);
    auto it = available.upper_bound(j);
    if (*--it == -1) {
      ans += r;
    } else {
      ans += l;
      available.erase(it);
    }
  }
  for (auto [gain, k, l, r] : righty) {
    i64 j = max<i64>(k, lefty.size());
    auto it = available.lower_bound(j);
    if (*it == N) {
      ans += l;
    } else {
      ans += r;
      available.erase(it);
    }
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    cout << solve() << '\n';
  }
}
