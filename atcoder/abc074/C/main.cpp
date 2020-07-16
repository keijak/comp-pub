#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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
  int a, b, c, d, e, f;
  cin >> a >> b >> c >> d >> e >> f;
  double max_density = -1.0;
  int ans_water = -1, ans_sugar = -1;
  set<int> waters;
  for (int ta = 0; ta <= f; ta += 100 * a) {
    for (int tb = 0; ta + tb <= f; tb += 100 * b) {
      waters.insert(ta + tb);
    }
  }
  set<int> sugars;
  for (int sc = 0; sc <= f; sc += c) {
    for (int sd = 0; sc + sd <= f; sd += d) {
      sugars.insert(sc + sd);
    }
  }
  for (int water : waters) {
    int max_sugar = e * water / 100;
    for (int sugar : sugars) {
      if (sugar > max_sugar) break;
      if (water + sugar > f) break;
      if (water + sugar == 0) continue;
      double density = 100.0 * sugar / double(water + sugar);
      if (max_density < density) {
        max_density = density;
        ans_water = water;
        ans_sugar = sugar;
      }
    }
  }

  cout << (ans_water + ans_sugar) << " " << ans_sugar << endl;
  DEBUG(max_density);
}
