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

// #include <boost/multiprecision/cpp_bin_float.hpp>
// using Float = boost::multiprecision::cpp_bin_float_oct;
using Float = long double;

optional<Float> solve() {
  int n, m, k;
  cin >> n >> m >> k;
  vector<bool> hole(100005);
  REP(i, k) {
    int a;
    cin >> a;
    hole[a] = true;
  }
  if (m <= k) {
    int c = 0;
    for (int i = 0; i < ssize(hole); ++i) {
      if (hole[i]) {
        ++c;
        if (c >= m) return nullopt;
      } else {
        c = 0;
      }
    }
  }

  vector dp(n + 1, vector(2, (Float)0.0));
  dp[n][0] = 0;  // const
  dp[n][1] = 0;  // *dp[0][1]

  vector cum(n + 1, vector(2, (Float)0.0));

  Float minv = 1.0 / Float(m);
  for (int i = n - 1; i >= 0; --i) {
    if (hole[i]) {
      dp[i][0] = 0;
      dp[i][1] = 1;
    } else {
      dp[i][0] = cum[i + 1][0] + m;
      dp[i][1] = cum[i + 1][1];
      dp[i][0] *= minv;
      dp[i][1] *= minv;
    }
    cum[i][0] = cum[i + 1][0] + dp[i][0];
    cum[i][1] = cum[i + 1][1] + dp[i][1];
    if (i + m <= n) {
      cum[i][0] -= dp[i + m][0];
      cum[i][1] -= dp[i + m][1];
    }
  }
  return Float(dp[0][0]) / (1 - dp[0][1]);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  auto res = solve();
  if (!res) {
    cout << "-1\n";
  } else {
    cout << (long double)(res.value()) << "\n";
  }
}
