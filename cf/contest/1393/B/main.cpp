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
  V<int> a(n);
  REP(i, n) cin >> a[i];
  int q;
  cin >> q;
  map<int, int> counter;
  multiset<pair<int, int>> rset;
  REP(i, n) {
    int c = counter[a[i]]++;
    auto it = rset.find({c, a[i]});
    if (it != rset.end()) {
      rset.erase(it);
    }
    rset.emplace(c + 1, a[i]);
  }

  auto solve = [&]() -> bool {
    char sign;
    int x;
    cin >> sign >> x;
    if (sign == '+') {
      int c = counter[x]++;
      auto it = rset.find({c, x});
      if (it != rset.end()) {
        rset.erase(it);
      }
      rset.emplace(c + 1, x);
    } else {
      int c = counter[x]--;
      if (c == 0) return false;
      auto it = rset.find({c, x});
      if (it != rset.end()) {
        rset.erase(it);
      }
      rset.emplace(c - 1, x);
    }

    // cerr << "=====" << endl;
    // for (auto [x, y] : rset) {
    //   DEBUG(x, y);
    // }

    auto i2 = rset.rbegin();
    if (i2 == rset.rend() || i2->first < 4) return false;
    int consumed = 4;

    if (i2->first - consumed >= 2) {
      consumed += 2;
    } else {
      ++i2;
      if (i2 == rset.rend() || i2->first < 2) return false;
      consumed = 2;
    }

    if (i2->first - consumed >= 2) {
      consumed += 2;
    } else {
      ++i2;
      if (i2 == rset.rend() || i2->first < 2) return false;
      consumed = 2;
    }

    return true;
  };

  REP(i, q) {
    DEBUG(i);
    cout << (solve() ? "YES" : "NO") << '\n';
  }
}
