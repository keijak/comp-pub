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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

i64 solve() {
  i64 n, x;
  cin >> n >> x;
  vector<i64> a(n);
  for (auto& x : a) cin >> x;
  sort(a.begin(), a.end());

  int k = n - 1;
  int cnt = 0;
  for (;;) {
    if (k < 0) return cnt;
    int c = 1;
    i64 v = a[k--];
    while (v * c < x) {
      if (k < 0) return cnt;
      v = a[k--];
      ++c;
    }
    ++cnt;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    DEBUG(t);
    cout << solve() << "\n";
  }
}
