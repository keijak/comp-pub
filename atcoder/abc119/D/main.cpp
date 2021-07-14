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

const i64 INF = 1LL << 40;

auto closest(const vector<i64>& v, i64 x) -> pair<i64, i64> {
  auto it = lower_bound(v.begin(), v.end(), x);
  i64 d1 = INF, d2 = INF, y1 = INF, y2 = INF;
  if (it != v.end()) {
    d1 = *it - x;
    assert(d1 >= 0);
    y1 = *it;
  }
  if (it != v.begin()) {
    --it;
    d2 = x - (*it);
    assert(d2 >= 0);
    y2 = *it;
  }
  if (d1 <= d2) {
    return {d1, y1};
  }
  return {d2, y2};
}

auto calc(const vector<i64>& v1, const vector<i64>& v2, i64 x) -> i64 {
  i64 res = INF;
  auto it = lower_bound(v1.begin(), v1.end(), x);
  if (it != v1.end()) {
    i64 d1 = *it - x;
    assert(d1 >= 0);
    auto [d2, z] = closest(v2, *it);
    res = min(res, d1 + d2);
  }
  if (it != v1.begin()) {
    --it;
    i64 d1 = x - (*it);
    assert(d1 >= 0);
    auto [d2, z] = closest(v2, *it);
    res = min(res, d1 + d2);
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int A, B, Q;
  cin >> A >> B >> Q;
  vector<i64> ps(A), pt(B);
  REP(i, A) cin >> ps[i];
  REP(i, B) cin >> pt[i];
  REP(i, Q) {
    i64 x;
    cin >> x;
    i64 res = min(calc(ps, pt, x), calc(pt, ps, x));
    cout << res << '\n';
  }
}
