#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
void chmax(T &a, T b) {
  if (a < b) a = std::move(b);
}
template <class T>
void chmin(T &a, T b) {
  if (a > b) a = std::move(b);
}

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
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

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
  int M, N;
  cin >> M >> N;
  string S, T;
  cin >> S >> T;
  auto dp = make_vec(2, 2, M + 1, N + 1, 0);
  for (int i = 0; i <= M; ++i) {
    for (int j = 0; j <= N; ++j) {
      if (i > 0 and S[i - 1] == 'I') {
        chmax(dp[1][0][i][j], 1);
      }
      if (j > 0 and T[j - 1] == 'I') {
        chmax(dp[0][1][i][j], 1);
      }
      if (i > 0 and j > 0 and
          ((S[i - 1] == 'I' and T[j - 1] == 'O') or
           (S[i - 1] == 'O' and T[j - 1] == 'I'))) {
        if (dp[0][1][i - 1][j - 1])
          chmax(dp[1][1][i][j], dp[0][1][i - 1][j - 1] + 2);
        if (dp[1][0][i - 1][j - 1])
          chmax(dp[1][1][i][j], dp[1][0][i - 1][j - 1] + 2);
        if (dp[1][1][i - 1][j - 1])
          chmax(dp[1][1][i][j], dp[1][1][i - 1][j - 1] + 2);
      }
      if (i >= 2 and S[i - 2] == 'O' and S[i - 1] == 'I') {
        if (dp[1][0][i - 2][j]) chmax(dp[1][0][i][j], dp[1][0][i - 2][j] + 2);
        if (dp[1][1][i - 2][j]) chmax(dp[1][1][i][j], dp[1][1][i - 2][j] + 2);
        if (dp[0][1][i - 2][j]) chmax(dp[1][1][i][j], dp[0][1][i - 2][j] + 2);
      }
      if (j >= 2 and T[j - 2] == 'O' and T[j - 1] == 'I') {
        if (dp[0][1][i][j - 2]) chmax(dp[0][1][i][j], dp[0][1][i][j - 2] + 2);
        if (dp[1][1][i][j - 2]) chmax(dp[1][1][i][j], dp[1][1][i][j - 2] + 2);
        if (dp[1][0][i][j - 2]) chmax(dp[1][1][i][j], dp[1][0][i][j - 2] + 2);
      }
    }
  }

  int ans = 0;
  REP(x, 2) REP(y, 2) REP(i, M + 1) REP(j, N + 1) {
    chmax(ans, dp[x][y][i][j]);
  }
  cout << ans << endl;
}
