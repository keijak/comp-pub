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
  int N;
  cin >> N;
  string S;
  cin >> S;
  int x = 0;
  deque<char> ans;
  REP(i, N) {
    if (S[i] == '(') {
      ++x;
      ans.push_back(S[i]);
    } else {
      assert(S[i] == ')');
      --x;
      if (x < 0) {
        ans.push_front('(');
        ++x;
      }
      ans.push_back(')');
    }
  }
  while (x > 0) {
    --x;
    ans.push_back(')');
  }
  string r(ans.begin(), ans.end());
  cout << r << '\n';
}