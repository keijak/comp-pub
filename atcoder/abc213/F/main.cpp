#include <bits/stdc++.h>
#include <atcoder/string>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

using namespace std;

template<typename T>
std::vector<int> min_range_left(const std::vector<T> &a) {
  const int n = a.size();
  std::stack<int> stk;
  std::vector<int> res(n);
  for (int i = n - 1; i >= -1; --i) {
    T val = (i >= 0) ? a[i] : std::numeric_limits<T>::lowest();
    while (not stk.empty() and a[stk.top()] >= val) {
      int j = stk.top();
      stk.pop();
      res[j] = i;
    }
    stk.push(i);
  }
  return res;
}

template<typename T>
std::vector<int> min_range_right(const std::vector<T> &a) {
  const int n = a.size();
  std::stack<int> stk;
  std::vector<int> res(n);
  for (int i = 0; i <= n; ++i) {
    T val = (i < n) ? a[i] : std::numeric_limits<T>::lowest();
    while (not stk.empty() and a[stk.top()] > val) {
      int j = stk.top();
      stk.pop();
      res[j] = i;
    }
    stk.push(i);
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  string s;
  cin >> n >> s;

  if (n == 1) {
    cout << 1 << "\n";
    return 0;
  }

  const vector<int> sa = atcoder::suffix_array(s);
  const vector<int> lcp = atcoder::lcp_array(s, sa);
  assert((int) lcp.size() == n - 1);
  vector<int> rsa(n);
  for (int i = 0; i < n; ++i) rsa[sa[i]] = i;

  auto lm = min_range_left(lcp);
  auto rm = min_range_right(lcp);

  vector<i64> lacc(n + 5), racc(n + 5);
  REP(i, n - 1) {
    i64 val = lcp[i] * i64(i - lm[i]);
    lacc[i + 1] += val;
    lacc[rm[i] + 1] -= val;
  }
  REP(i, n - 1) {
    i64 val = lcp[i] * i64(rm[i] - i);
    racc[lm[i] + 1] += val;
    racc[i + 1] -= val;
  }
  REP(i, n) {
    lacc[i + 1] += lacc[i];
    racc[i + 1] += racc[i];
  }
  REP(k, n) {
    int i = rsa[k];
    i64 ans = (n - k) + lacc[i] + racc[i];
    cout << ans << "\n";
  }
}
