#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string S;
  cin >> S;
  const int n = SIZE(S);

  V<i64> pow10(n + 1);
  pow10[0] = 1;
  REP(i, n) { pow10[i + 1] = pow10[i] * 10; }
  V<i64> leftnum(n + 1), rightnum(n + 1);
  leftnum[0] = 0;
  for (int i = 0; i < n; ++i) {
    leftnum[i + 1] = leftnum[i] * 10 + (S[i] - '0');
  }
  rightnum[n] = 0;
  for (int i = 0; i < n; ++i) {
    int ri = n - 1 - i;
    rightnum[ri] = rightnum[ri + 1] + (S[ri] - '0') * pow10[i];
  }

  auto table = vector(n + 1, 0LL);
  for (int i = 1; i <= n; ++i) {
    int ri = n - i;
    int sd = S[ri] - '0';
    DEBUG(i, sd, leftnum[ri], rightnum[ri + 1]);
    i64 left = leftnum[ri];
    if (sd == 0) {
      table[i] += left * pow10[i - 1];
    } else if (sd > 1) {
      table[i] += (left + 1) * pow10[i - 1];
    } else {
      table[i] += left * pow10[i - 1];
      table[i] += rightnum[ri + 1] + 1;
    }
  }
  i64 ans = accumulate(ALL(table), 0LL);
  cout << ans << endl;
}
