//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, L;
  cin >> N >> L;
  vector<tuple<int, int, i64>> lights(N);
  REP(i, N) {
    int l, r;
    i64 c;
    cin >> l >> r >> c;
    lights[i] = make_tuple(l, r, c);
  }
  sort(lights.begin(), lights.end());

  map<int, i64> t;
  t[0] = 0LL;
  REP(i, N) {
    const auto& light = lights[i];
    int l = get<0>(light);
    int r = get<1>(light);
    i64 c = get<2>(light);
    auto lit = t.lower_bound(l);
    assert(lit != t.end());
    assert(l <= lit->first);
    i64 before = lit->second;

    DEBUG(i, l, r, c, lit->first, lit->second);
    auto rit = t.lower_bound(r);
    if (rit == t.end()) {
      t[r] = before + c;
    } else if (rit->second > before + c) {
      t[r] = before + c;
    }
    int ng = l;  // -> before
    int ok = r;  // -> before + c
    while (ok - ng > 1) {
      int m = (ok + ng) / 2;
      auto it = t.lower_bound(m);
      if (it->second >= before + c) {
        ok = m;
      } else {
        ng = m;
      }
    }
    auto eit1 = t.lower_bound(ok);
    auto eit2 = t.lower_bound(r);
    if (eit1 != eit2) {
      t.erase(eit1, eit2);
    }
    // for (int j = 0; j <= L; ++j) {
    //   auto it = t.lower_bound(j);
    //   if (it == t.end()) {
    //     cerr << "\t_";
    //   } else {
    //     cerr << "\t" << it->second;
    //   }
    // }
    // cerr << endl;
  }
  i64 ans = t[L];
  cout << ans << endl;
}
