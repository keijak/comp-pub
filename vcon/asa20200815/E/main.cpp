#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  V<tuple<int, bool, int>> events;
  REP(i, n) {
    int a, b;
    cin >> a >> b;
    events.emplace_back(a, true, b);
  }
  REP(i, n) {
    int c, d;
    cin >> c >> d;
    events.emplace_back(c, false, d);
  }
  sort(events.begin(), events.end());
  set<int> s;
  int cnt = 0;
  for (auto [x, is_red, y] : events) {
    if (is_red) {
      s.insert(y);
    } else {
      auto it = s.lower_bound(y);
      if (it == s.begin()) continue;
      --it;
      s.erase(it);
      ++cnt;
    }
  }
  cout << cnt << endl;
}
