#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

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

#ifdef ENABLE_DEBUG
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

const int M = 5'000'005;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<bool> is_prime(M, true);
  vector<i64> fcount(M, 0), fcum(M + 1);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i < M; i += 2) {
    is_prime[i] = false;
    int x = i;
    while (not(x & 1)) {
      fcount[i]++;
      x >>= 1;
    }
  }
  for (int i = 3; i < M; i += 2) {
    if (!is_prime[i]) continue;
    for (int j = 2 * i; j < M; j += i) {
      is_prime[j] = false;
      int x = j;
      while (x % i == 0) {
        fcount[j]++;
        x /= i;
      }
    }
  }
  REP(i, M) {
    if (is_prime[i]) fcount[i] = 1;
  }

  // [0, i)
  REP(i, M) { fcum[i + 1] = fcum[i] + fcount[i]; }

  auto solve = [&]() -> i64 {
    int a, b;
    cin >> a >> b;
    return fcum[a + 1] - fcum[b + 1];
  };

  int t;
  cin >> t;
  REP(i, t) { cout << solve() << '\n'; }
}
