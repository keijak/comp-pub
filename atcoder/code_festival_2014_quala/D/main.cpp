#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), N_##i = (b); i < N_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;
const i64 INF = 1e18;

i64 solve() {
  i64 A;
  int K;
  cin >> A >> K;

  stringstream ss;
  ss << A;
  string SA = ss.str();
  {
    set<char> cs;
    for (char c : SA) cs.insert(c);
    if (ssize(cs) <= K) return 0;
  }
  int n = ssize(SA);

  i64 ans = A;
  vector<int> digits(10, 0);
  REP(i, K) digits[9 - i] = 1;
  do {
    DEBUG(digits);
    {
      vector dp(n + 1, vector(10, vector(2, vector(2, -1LL))));
      dp[0][0][0][1] = 0LL;
      REP(i, n) REP(d, 10) REP(nd, 10) REP(less, 2) REP(leading, 2) {
        if (dp[i][d][less][leading] == -1) continue;
        int nless = less;
        int nleading = leading;
        if (leading and nd == 0) {
          // ok
        } else {
          if (not digits[nd]) continue;
          nleading = 0;
        }
        int sd = SA[i] - '0';
        if (not less and nd > sd) continue;
        if (not less and nd < sd) nless = 1;
        chmax(dp[i + 1][nd][nless][nleading],
              dp[i][d][less][leading] * 10LL + nd);
      }
      i64 maxi = 0;
      REP(d, 10) {
        auto x = dp[n][d][1][0];
        if (x != -1) chmax(maxi, x);
      }
      assert(A >= maxi);
      chmin(ans, A - maxi);
    }
    {
      vector dp(n + 1, vector(10, vector(2, vector(2, INF))));
      dp[0][0][0][1] = 0LL;
      REP(i, n) REP(d, 10) REP(nd, 10) REP(bigger, 2) REP(leading, 2) {
        if (dp[i][d][bigger][leading] == INF) continue;
        int nbigger = bigger;
        int nleading = leading;
        if (leading and nd == 0) {
          // ok
        } else {
          if (not digits[nd]) continue;
          nleading = 0;
        }
        int sd = SA[i] - '0';
        if (not bigger and nd < sd) continue;
        if (not bigger and nd > sd) nbigger = 1;
        chmin(dp[i + 1][nd][nbigger][nleading],
              dp[i][d][bigger][leading] * 10LL + nd);
      }
      i64 mini = INF;
      REP(d, 10) {
        auto x = dp[n][d][1][0];
        if (x != INF) chmin(mini, x);
      }
      if (mini < INF) {
        DEBUG(mini);
        assert(mini >= A);
        chmin(ans, mini - A);
      }
    }
  } while (next_permutation(ALL(digits)));

  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
