#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  sort(A.begin(), A.end());
  set<vector<pair<i64, int>>> s;

  auto factorize = [&](i64 x, vector<pair<i64, int>>& fs) -> void {
    for (i64 p = 2; p * p <= x; ++p) {
      int cnt = 0;
      while (x % p == 0) {
        x /= p;
        cnt++;
      }
      if (cnt) {
        fs.emplace_back(p, cnt);
      }
    }
    if (x > 1) fs.emplace_back(x, 1);
  };

  auto divi = [&](auto rec, vector<pair<i64, int>>& fs) -> bool {
    if (s.find(fs) != s.end()) return true;
    vector<pair<i64, int>> tmp(fs.begin(), fs.end());
    REP(i, fs.size()) {
      auto it = tmp.begin() + (tmp.size() - 1 - i);
      if (it->second == 1) {
        tmp.erase(it);
      } else {
        it->second--;
      }
      if (rec(rec, tmp)) {
        s.insert(fs);
        return true;
      }
    }
    return false;
  };

  int ans = 0;
  REP(i, N) {
    vector<pair<i64, int>> fs;
    factorize(A[i], fs);
    if (!divi(divi, fs)) {
      DEBUG(i);
      ++ans;
    }
  }
  cout << ans << endl;
}
