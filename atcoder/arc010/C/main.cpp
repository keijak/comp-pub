#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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
const i64 INF = 1e12;

i64 solve() {
  i64 n, m, Y, Z;
  cin >> n >> m >> Y >> Z;
  vector<int> color_index(26, -1);
  vector<int> color_score(m, -500);
  REP(i, m) {
    char c;
    int p;
    cin >> c >> p;
    int color = c - 'A';
    color_index[color] = i;
    assert(color_score[i] == -500);
    color_score[i] = p;
  }
  string b;
  cin >> b;
  assert(ssize(b) == n);
  const unsigned complete_mask = (1 << m) - 1;

  vector dp(2, vector(1 << m, vector(m + 1, -INF)));
  dp[0][0][m] = 0;

  REP(i, n) {
    int color = b[i] - 'A';
    int cindex = color_index[color];
    assert(0 <= cindex and cindex < m);
    i64 cscore = color_score[cindex];
    REP(having, 1 << m) {
      REP(top, m + 1) {
        if (dp[0][having][top] == -INF) continue;
        chmax(dp[1][having][top], dp[0][having][top]);
        i64 this_score = cscore;
        unsigned new_having = having | (1 << cindex);
        if ((having & complete_mask) != complete_mask and
            (new_having & complete_mask) == complete_mask) {
          this_score += Z;
        }
        if (top == cindex) {
          this_score += Y;
        }
        i64 val = dp[0][having][top] + this_score;
        chmax(dp[1][new_having][cindex], val);
      }
    }
    // DEBUG(i, b[i], cindex, cscore, ans);
    swap(dp[0], dp[1]);
    REP(j, 1 << m) { fill(ALL(dp[1][j]), -INF); }
  }

  i64 ans = 0;
  REP(having, 1 << m) REP(top, m + 1) { chmax(ans, dp[0][having][top]); }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
