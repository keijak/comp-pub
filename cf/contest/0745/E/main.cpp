#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
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

struct Card {
  bool is_red;
  i64 r;
  i64 b;
};
bool operator<(const Card &x, const Card &y) {
  return (x.r + x.b) < (y.r + y.b);
};

const int INF = 1e9;

i64 solve() {
  int n;  //<= 16
  cin >> n;
  vector<Card> cards;
  i64 total_r = 0, total_b = 0;
  REP(i, n) {
    char c;
    cin >> c;
    i64 r, b;
    cin >> r >> b;
    cards.push_back({c == 'R', r, b});
    total_r += r;
    total_b += b;
  }

  auto dp = vector(1 << n, vector(2, vector(n * n, INF)));
  dp[0][0][0] = dp[0][1][0] = 0;

  REP(s, 1 << n) {
    i64 nr = 0, nb = 0;
    i64 cost_r = 0, cost_b = 0;
    vector<int> purchased;
    REP(i, n) {
      if (s >> i & 1) {
        if (cards[i].is_red) {
          ++nr;
        } else {
          ++nb;
        }
        cost_r += cards[i].r;
        cost_b += cards[i].b;
        purchased.push_back(i);
      }
    }

    REP(red_empty, 2) {
      REP(reduction, n * n) {
        i64 ndraw = dp[s][red_empty][reduction];
        if (ndraw == INF) continue;
        i64 token_r = red_empty ? 0 : ndraw - cost_r + reduction;
        i64 token_b = red_empty ? ndraw - cost_b + reduction : 0;
        for (int p = 0; p < n; ++p) {
          if (s & (1 << p)) continue;
          const auto &cp = cards[p];
          i64 after_r = token_r - max(cp.r - nr, 0LL);
          i64 after_b = token_b - max(cp.b - nb, 0LL);
          i64 draw_now = -min(after_r, after_b);
          assert(draw_now >= 0);
          after_r += draw_now;
          after_b += draw_now;
          assert(after_r == 0 or after_b == 0);
          if (after_r == 0) {
            i64 reduction2 = after_b + (cost_b + cp.b) - (ndraw + draw_now);
            chmin(dp[s | (1 << p)][1][reduction2], int(ndraw + draw_now));
          }
          if (after_b == 0) {
            i64 reduction2 = after_r + (cost_r + cp.r) - (ndraw + draw_now);
            chmin(dp[s | (1 << p)][0][reduction2], int(ndraw + draw_now));
          }
        }
      }
    }
  }
  i64 full = (1 << n) - 1;
  int min_collect = INF;
  REP(i, 2) REP(j, n * n) { chmin(min_collect, dp[full][i][j]); }
  return min_collect + n;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
