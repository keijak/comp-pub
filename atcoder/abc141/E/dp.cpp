#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  string s;
  cin >> n >> s;

  int ans = 0;
  auto dp = make_vec(n + 1, 0);
  for (int i = n - 2; i >= 0; --i) {
    auto dp_next = make_vec(n + 1, 0);
    for (int j = i + 1; j < n; ++j) {
      if (s[i] == s[j]) {
        dp_next[j] = dp[j + 1] + 1;
        chmax(ans, min(dp_next[j], j - i));
      }
    }
    swap(dp, dp_next);
  }
  cout << ans << endl;
}
