#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
using Float = long double;

Float solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  cin >> a;

  vector<i64> pow3(n + 1);
  pow3[0] = 1;
  REP(i, n) pow3[i + 1] = pow3[i] * 3;
  DEBUG(pow3);

  vector dp(pow3[n], vector(2, (Float)0));
  vector dp2(pow3[n], vector(2, vector(n, vector(n, (Float)0))));

  const unsigned full = (1 << n) - 1;
  for (unsigned sub = full;; sub = (sub - 1) & full) {
    int p1 = 0, p2 = 0;
    int state = 0;
    REP(i, n) {
      if (sub & (1 << i)) {
        p1 += a[i];
        state += pow3[i];
      } else {
        p2 += a[i];
        state += pow3[i] * 2;
      }
    }
    REP(i, 2) { dp[state][i] = (p1 > p2) ? 1.0 : 0.0; }
    if (sub == 0) break;
  }

  REP(bits, 1, 1 << n) {
    const unsigned taken = (~bits) & full;
    for (unsigned sub = taken;; sub = (sub - 1) & taken) {
      i64 state = 0;
      REP(j, n) {
        if (sub & (1 << j)) {
          state += pow3[j];
        } else if (taken & (1 << j)) {
          state += pow3[j] * 2;
        }
      }
      REP(i, n) {
        if (taken & (1 << i)) continue;
        REP(j, n) {
          if (taken & (1 << j)) continue;
          Float qi = 1.0 / a[i], qj = 1.0 / a[j];
          const Float denom = 1.0 - (1.0 - qi) * (1.0 - qj);
          dp2[state][0][i][j] = (qi * dp[state + pow3[i]][1] +
                                 (1.0 - qi) * qj * dp[state + pow3[j] * 2][0]) /
                                denom;
          dp2[state][1][i][j] = (qj * dp[state + pow3[j] * 2][0] +
                                 (1.0 - qj) * qi * dp[state + pow3[i]][1]) /
                                denom;
        }
      }
      dp[state][0] = 0.0;
      dp[state][1] = 1.0;
      REP(i, n) {
        if (taken & (1 << i)) continue;
        Float minp = 1.0, maxp = 0.0;
        REP(j, n) {
          if (taken & (1 << j)) continue;
          chmin(minp, dp2[state][0][i][j]);
          chmax(maxp, dp2[state][1][j][i]);
        }
        chmax(dp[state][0], minp);
        chmin(dp[state][1], maxp);
      }
      if (sub == 0) break;
    }
  }
  return dp[0][0];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  cout << solve() << "\n";
}
