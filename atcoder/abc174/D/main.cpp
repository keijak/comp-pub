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
  auto solve = [&]() -> int {
    int head = 0, tail = N - 1;
    int cost = 0;
    for (;;) {
      while (head < tail && S[head] == 'R') {
        ++head;
      }
      if (head == tail) break;
      while (tail > head && S[tail] == 'W') {
        --tail;
      }
      if (head == tail) break;
      assert(head < tail);
      swap(S[head], S[tail]);
      ++cost;
      DEBUG(cost, S);
    }

    return cost;
  };
  cout << solve() << endl;
}
