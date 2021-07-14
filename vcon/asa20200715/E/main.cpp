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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<tuple<int, int, bool>> ps(2 * N);
  REP(i, 2 * N) {
    int x, y;
    cin >> x >> y;
    ps[i] = {x, y, i < N};
  }
  sort(ps.begin(), ps.end());
  set<int> red;
  int count = 0;
  REP(i, 2 * N) {
    auto [x, y, is_red] = ps[i];
    if (is_red) {
      red.insert(y);
    } else if (red.size()) {
      auto it = red.lower_bound(y);
      if (it == red.begin()) continue;
      --it;
      assert(*it < y);
      ++count;
      red.erase(it);
    }
  }
  cout << count << endl;
}
