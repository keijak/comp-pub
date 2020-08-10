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
  int N;
  cin >> N;

  map<int, map<int, int>> mp;
  i64 ans = 0;
  REP(i, N) {
    string s;
    cin >> s;
    size_t j = s.find('.');
    i64 x;
    int d;
    if (j == string::npos) {
      x = stoll(s);
      d = 0;
    } else {
      string a = s.substr(0, j);
      string b = s.substr(j + 1);
      d = -b.size();
      x = stoll(a + b);
    }

    int d2 = d;
    while (x % 2 == 0) {
      x /= 2;
      ++d2;
    }
    int d5 = d;
    while (x % 5 == 0) {
      x /= 5;
      ++d5;
    }
    auto it1 = mp.lower_bound(-d5);
    for (; it1 != mp.end(); ++it1) {
      auto it2 = it1->second.lower_bound(-d2);
      for (; it2 != it1->second.end(); ++it2) {
        ans += it2->second;
      }
    }
    mp[d5][d2]++;
  }
  cout << ans << endl;
}
