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
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ",", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ",", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

#define chmax(x, y) x = max(x, y)

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  int N3 = 3 * N;
  V<int> A(N3);
  REP(i, N3) cin >> A[i];
  const int INF = 1e8;

  int ans = [&]() -> i64 {
    if (N == 1) {
      if (A[0] == A[1] && A[1] == A[2]) return 1;
      return 0;
    }

    V<V<V<int>>> dp = make_vec(N, N + 1, N + 1, -INF);
    int a0 = min(A[0], A[1]);
    int a1 = max(A[0], A[1]);
    dp[0][a0][a1] = 0;
    for (int i = 1; i < N; ++i) {
      for (int j = 1; j <= N; ++j) {
        for (int k = j; k <= N; ++k) {
          if (dp[i - 1][j][k] == INF) continue;
          int hand[5];
          REP(p, 3) { hand[p] = A[3 * i - 1 + p]; }
          hand[3] = j;
          hand[4] = k;
          sort(hand, hand + 5);

          int ok = -1;
          REP(p, 3) {
            if (hand[p] == hand[p + 1] && hand[p] == hand[p + 2]) {
              ok = p;
              break;
            }
          }

          for (int p = 0; p < 4; ++p) {
            for (int q = p + 1; q < 5; ++q) {
              chmax(dp[i][hand[p]][hand[q]], dp[i - 1][j][k]);
            }
          }

          if (ok >= 0) {
            V<int> tmp;
            REP(p, 5) {
              if (p < ok || p >= ok + 3) tmp.push_back(hand[p]);
            }
            chmax(dp[i][tmp[0]][tmp[1]], dp[i - 1][j][k] + 1);
          }
        }
      }
    }

    REP(i, N) REP(j, N + 1) { DEBUG(i, j, dp[i][j]); }

    int ans = -INF;
    REP(i, N + 1) REP(j, N + 1) {
      int bonus = 0;
      if (i == j && j == A[N3 - 1]) bonus = 1;
      int res = dp[N - 1][i][j] + bonus;
      if (ans < res) {
        DEBUG(res, i, j, bonus);
        ans = res;
      }
    }
    return ans;
  }();
  cout << ans << endl;
}
