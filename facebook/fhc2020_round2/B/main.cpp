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

void solve(int cas) {
  // input
  int N;     // < 8000
  double P;  // [0.5, 1.0]
  cin >> N >> P;

  V<V<double>> dp(N, V<double>(N, -1e18));
  REP(i, N) { dp[0][i] = 1.0; }
  for (int i = 1; i < N; ++i) {
    // N+1-i people are remaining.
    V<double> lcum(N + 1, 0.0), rcum(N + 1, 0.0);
    REP(j, N) {
      lcum[j + 1] = lcum[j] + dp[i - 1][j];
      rcum[N - 1 - j] = rcum[N - j] + dp[i - 1][N - 1 - j];
    }
    REP(j, N) {
      double p_unselected = double(N - i - 1) / double(N - i + 1);
      double p_selected = 2.0 / double(N - i + 1);
      double p_win = (P * lcum[j] + (1.0 - P) * rcum[j + 1]) /
                     double(lcum[j] + rcum[j + 1]);
      dp[i][j] = dp[i - 1][j] * (p_unselected + p_selected * p_win);
    }
  }

  DEBUG(cas);
  REP(i, N) DEBUG(i, dp[i]);

  // Output N lines.
  REP(j, N) {
    double ans = 0.0;
    for (int i = 0; i < N - 1; ++i) {
      ans += dp[i][j];
    }
    /*
    for (int i = 1; i < N; ++i) {
      double q = (i == N - 1) ? 1.0 : (1.0 - dp[i][j]);
      ans += double(i) * dp[i - 1][j] * q;
    }
    */
    cout << ans << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << fixed << setprecision(8);
  int T;
  cin >> T;
  REP(i, T) {
    cout << "Case #" << (i + 1) << ":\n";
    solve(i + 1);
  }
}
