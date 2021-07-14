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
  i64 n, m;
  cin >> n >> m;
  set<i64> fs;
  for (int i = 2; i * i <= m; ++i) {
    if (m % i == 0) {
      fs.insert(i);
      fs.insert(m / i);
    }
  }
  fs.insert(m);
  auto find = [&]() -> i64 {
    for (auto it = fs.rbegin(); it != fs.rend(); ++it) {
      i64 g = *it;
      if (n <= m / g) return g;
    }
    return 1;
  };
  cout << find() << endl;
}
