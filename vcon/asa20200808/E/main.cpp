#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  i64 t1, t2;
  cin >> t1 >> t2;
  i64 a1, a2, b1, b2;
  cin >> a1 >> a2 >> b1 >> b2;
  i64 d = a1 * t1 + a2 * t2 - (b1 * t1 + b2 * t2);
  if (d == 0) {
    cout << "infinity" << endl;
    return 0;
  }

  // Ensure a1*t1 + a2*t2 is bigger.
  if (d < 0) {
    swap(a1, b1);
    swap(a2, b2);
    d *= -1;
  }

  i64 e = b1 * t1 - a1 * t1;
  if (e < 0) {
    cout << 0 << endl;
    return 0;
  }

  i64 k = e / d;
  if (e % d == 0) {
    cout << 2 * k << endl;
  } else {
    cout << 2 * k + 1 << endl;
    //   i64 k = e / d;

    //   cout << 2LL * k + 1 << endl;
  }
}
