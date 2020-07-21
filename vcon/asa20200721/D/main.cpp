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
  map<char, int> trans = {{'b', 1}, {'c', 1}, {'d', 2}, {'w', 2}, {'t', 3},
                          {'j', 3}, {'f', 4}, {'q', 4}, {'l', 5}, {'v', 5},
                          {'s', 6}, {'x', 6}, {'p', 7}, {'m', 7}, {'h', 8},
                          {'k', 8}, {'n', 9}, {'g', 9}, {'z', 0}, {'r', 0}};

  int n;
  cin >> n;
  bool first = true;
  REP(i, n) {
    string word;
    cin >> word;
    string num;
    REP(j, word.size()) {
      char ch = tolower(word[j]);
      auto it = trans.find(ch);
      if (it == trans.end()) continue;
      int x = it->second;
      num.push_back('0' + x);
    }
    if (num.empty()) continue;
    if (!first) cout << " ";
    first = false;
    cout << num;
  }
  cout << endl;
}
