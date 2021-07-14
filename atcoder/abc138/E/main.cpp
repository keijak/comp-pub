#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}

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

  string s, t;
  cin >> s >> t;

  V<V<int>> dic(26);
  REP(i, s.size()) {
    int c = s[i] - 'a';
    dic[c].push_back(i);
  }

  i64 ans = [&]() -> i64 {
    i64 iter = 0;
    i64 j = 0;
    REP(i, t.size()) {
      int tc = t[i] - 'a';
      const V<int> &d = dic[tc];
      if (d.empty()) return -1;
      while (true) {
        auto it = lower_bound(d.begin(), d.end(), j);
        if (it == d.end()) {
          assert(j > 0);
          ++iter;
          j = 0;
          continue;
        } else {
          j = *it + 1;
          break;
        }
      }
    }
    return iter * s.size() + j;
  }();
  cout << ans << endl;
}
