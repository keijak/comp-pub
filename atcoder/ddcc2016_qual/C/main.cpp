#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), end__##i = (b); i < end__##i; ++i)
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

#ifdef MY_DEBUG
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

int a[200005];
pair<int, int> divisors[1500];

bool pair_less(const pair<int, int> &x, const pair<int, int> &y) {
  return x.first < y.first;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, k;
  cin >> n >> k;
  REP(i, n) cin >> a[i];

  unordered_map<int, int> gcds;
  gcds.reserve(1 << 11);
  gcds.max_load_factor(0.25);
  REP(i, n) {
    int g = std::gcd(a[i], k);
    ++gcds[g];
  }

  int m = 0;
  for (int i = 1; i * i <= k; ++i) {
    const auto &[q, r] = std::div(k, i);
    if (r != 0) continue;
    divisors[m++] = pair(i, gcds[i]);
    if (q != i) divisors[m++] = pair(q, gcds[q]);
  }
  sort(divisors, divisors + m, pair_less);

  i64 ans = 0;
  for (int i = m - 1; i >= 0; --i) {
    const auto &[di, ci] = divisors[i];
    if (ci == 0) continue;
    i64 di2 = i64(di) * di;
    if (di2 < k) break;
    if (di2 % k == 0) {
      ans += i64(ci) * (ci - 1) / 2;
    }
    for (int j = i - 1; j >= 0; --j) {
      const auto &[dj, cj] = divisors[j];
      i64 dd = i64(di) * dj;
      if (dd < k) break;
      if (dd % k == 0) {
        ans += i64(ci) * cj;
      }
    }
  }
  cout << ans << "\n";
}
