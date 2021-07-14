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

bool is_753(i64 x) {
  bool has7 = false, has5 = false, has3 = false;
  while (x) {
    int d = x % 10;
    if (d == 7)
      has7 = true;
    else if (d == 5)
      has5 = true;
    else if (d == 3)
      has3 = true;
    else
      return false;
    x /= 10;
  }
  return has7 && has5 && has3;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;

  int cnt = 0;
  auto f = [&](auto rec, i64 base, i64 x) -> void {
    if (x > N) return;
    if (is_753(x)) ++cnt;
    for (i64 c : {7, 5, 3}) {
      rec(rec, base * 10, c * base + x);
    }
  };
  f(f, 1LL, 0LL);
  cout << cnt << endl;
}
