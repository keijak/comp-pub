#define ENABLE_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
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
  int N;
  cin >> N;
  string S;
  cin >> S;
  int Q;
  cin >> Q;
  vector<int> K(Q);
  for (auto& x : K) cin >> x;

  REP(i, Q) {
    const i64 k = K[i];
    int head = 0, tail = 0;
    i64 nd = 0, nm = 0, nc = 0;
    while (head < N && S[head] != 'D') ++head;
    tail = head;
    i64 ans = 0;
    while (true) {
      for (; tail < head + k; tail++) {
        if (tail == N) break;
        if (S[tail] == 'D') nd++;
        if (S[tail] == 'M') nm++;
        if (S[tail] == 'C') nc++;
      }
      ans += nd * nm * nc;
    }
  }
}
