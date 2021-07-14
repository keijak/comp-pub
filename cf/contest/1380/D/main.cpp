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

const i64 INF = 1LL << 50;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 n, m;
  cin >> n >> m;
  i64 x, k, y;
  cin >> x >> k >> y;
  vector<i64> a(n), b(m);
  for (auto& z : a) cin >> z;
  for (auto& z : b) cin >> z;
  map<i64, int> amp;
  REP(i, n) { amp[a[i]] = i; }

  auto calc_mana = [&](int l, int r, i64 bx) -> i64 {
    int width = r - l;
    if (width == 0) {
      return 0;
    }

    i64 manacost = INF;
    if (width >= k) {
      i64 c = (width % k) * y + (width / k) * x;
      manacost = min(manacost, c);
    }

    int fb_count = 0;
    int killed = 0;
    int remain = 0;
    for (int i = l; i < r; ++i) {
      if (i < killed) continue;
      ++remain;
      if (a[i] <= bx) continue;
      ++fb_count;
      killed = i + k;
    }
    if (killed > r) {
      remain -= killed - r;
    }
    if (remain >= 0) {
      i64 c2 = fb_count * x + remain * y;
      manacost = min(manacost, c2);
    }
    return manacost;
  };

  auto calc = [&]() -> i64 {
    int j = 0;
    i64 manacost = 0;
    REP(i, m) {
      auto ait = amp.find(b[i]);
      if (ait == amp.end()) return INF;
      int ai = ait->second;
      assert(a[ai] == b[i]);
      if (ai < j) return INF;
      i64 ub = b[i];
      if (i > 0) ub = max(b[i], b[i - 1]);
      i64 cost = calc_mana(j, ai, ub);
      if (cost == INF) {
        return INF;
      }
      manacost += cost;
      j = ai + 1;
    }
    i64 fin_cost = calc_mana(j, n, b[m - 1]);
    if (fin_cost == INF) return INF;
    return manacost + fin_cost;
  };

  i64 ans = calc();  // min mana
  cout << (ans == INF ? -1 : ans) << endl;
}
