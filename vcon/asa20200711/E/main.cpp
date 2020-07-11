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
  i64 N;
  cin >> N;
  string S;
  cin >> S;
  int Q;
  cin >> Q;

  vector<set<int>> ix(26);
  REP(i, N) {
    int ch = S[i] - 'a';
    ix[ch].insert(i);
  }

  REP(q, Q) {
    int tp;
    cin >> tp;
    if (tp == 1) {
      int iq;
      char cq;
      cin >> iq >> cq;
      iq--;
      int before = S[iq] - 'a';
      int after = cq - 'a';
      if (before != after) {
        ix[before].erase(iq);
        ix[after].insert(iq);
      }
      S[iq] = cq;
    } else {
      assert(tp == 2);
      int l, r;
      cin >> l >> r;
      l--;
      r--;
      int cnt = 0;
      REP(i, 26) {
        auto it = ix[i].lower_bound(l);
        if (it == ix[i].end()) continue;
        if (*it <= r) ++cnt;
      }
      cout << cnt << '\n';
    }
  }
}
