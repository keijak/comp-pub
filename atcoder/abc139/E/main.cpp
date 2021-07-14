#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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
  int N;
  cin >> N;
  vector<vector<int>> A(N, vector<int>(N - 1));
  REP(i, N) REP(j, N - 1) {
    int x;
    cin >> x;
    A[i][j] = x - 1;
  }
  vector<int> counter(N);
  vector<tuple<int, int>> pairs;
  REP(i, N) {
    int opponent = A[i][0];
    if (i < opponent && A[opponent][0] == i) {
      pairs.emplace_back(i, opponent);
    }
  }
  auto solve = [&]() -> int {
    int days = 0;
    int rem_count = N * (N - 1);
    vector<tuple<int, int>> tmp;
    for (; rem_count > 0; ++days) {
      if (pairs.empty()) return -1;
      tmp.clear();
      swap(tmp, pairs);
      for (auto [x1, x2] : tmp) {
        counter[x1]++;
        counter[x2]++;
        rem_count -= 2;
        for (int i : {x1, x2}) {
          if (counter[i] >= N - 1) continue;
          int opponent = A[i][counter[i]];
          int k = counter[opponent];
          assert(k < N - 1);
          if (A[opponent][k] == i) {
            pairs.emplace_back(min(i, opponent), max(i, opponent));
          }
        }
      }
    }
    return days;
  };
  cout << solve() << endl;
}
