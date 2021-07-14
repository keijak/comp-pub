//#define DEBUGGING  // Enables DEBUG macro.
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
  int Q;
  cin >> Q;
  REP(k, Q) {
    string S;
    cin >> S;
    sort(S.rbegin(), S.rend());
    int M;
    cin >> M;
    vector<int> B(M);
    for (auto& x : B) cin >> x;
    vector<pair<int, int>> bx(M);
    REP(i, M) { bx[i] = {B[i], i}; }
    sort(bx.begin(), bx.end());
    // REP(i, M) { DEBUG(k, bx[i].first, bx[i].second); }
    assert(bx[0].first == 0);
    DEBUG(k, S);
    vector<char> T(M);
    auto dbgt = [&](int bxi, int si) -> void {
      DEBUG(k, bxi, si);
      REP(j, M) {
        if (T[j] == 0)
          cerr << '_';
        else
          cerr << T[j];
      }
      cerr << endl;
    };
    auto is_ok = [&](int diffs, int ti, char x) -> bool {
      REP(j, M) {
        if (T[j] != 0 && T[j] > x) {
          diffs -= abs(ti - j);
          if (diffs < 0) return false;
        }
      }
      return diffs == 0;
    };
    auto dfs = [&](auto rec, int si, int filled) -> bool {
      if (filled == M) return true;
      if (si == (int)S.size()) return false;
      REP(bi, M) {
        if (T[bi] == 0 && is_ok(B[bi], bi, S[si])) {
          T[bi] = S[si];
          // dbgt(bi, si);
          if (rec(rec, si + 1, filled + 1)) return true;
          T[bi] = 0;
        }
      }
      return rec(rec, si + 1, filled);
    };
    bool result = dfs(dfs, 0, 0);
    assert(result);

    string ans(T.begin(), T.end());
    cout << ans << '\n';
  }
}
