#define MY_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if MY_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string s, t;
  getline(cin, s);
  getline(cin, t);
  int tn = t.size();

  bool unrestorable = true;
  for (int i = s.size() - 1; i >= tn - 1; --i) {
    bool matched = true;
    for (int j = 0; j < tn; ++j) {
      if (s[i - j] != '?' && s[i - j] != t[tn - 1 - j]) {
        matched = false;
        break;
      }
    }
    if (matched) {
      unrestorable = false;
      for (int j = 0; j < tn; ++j) {
        s[i - j] = t[tn - 1 - j];
      }
      break;
    }
  }
  if (unrestorable) {
    cout << "UNRESTORABLE\n";
  } else {
    REP(i, s.size()) {
      if (s[i] == '?') s[i] = 'a';
    }
    cout << s << '\n';
  }
}
