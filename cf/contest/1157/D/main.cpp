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

bool possible(i64 n, i64 k) {
  if (n < k * (k + 1) / 2) return false;

  i64 x = 1;
  REP(i, k) {
    if (x >= n) return true;
    x *= 2;
  }
  return false;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 n, k;
  cin >> n >> k;

  deque<i64> a(k + 1);

  //   if (n == k * (k + 1) / 2) {
  //     REP(i, k) a[i] = i + 1;
  //     a.pop_back();
  //     pprint(a);
  //     exit(0);
  //   }

  a[0] = 0;
  i64 v = n;
  for (int i = 1; i <= k; ++i) {
    i64 r = k - i + 1;
    i64 rmin = r * (2 * (a[i - 1] + 1) + r - 1) / 2LL;
    if (v < rmin) {
      cout << "NO\n";
      exit(0);
    }
    assert(v >= rmin);
    if (r == 1) {
      if (v < a[i - 1] + 1 or (i > 1 and v > a[i - 1] * 2)) {
        cout << "NO\n";
        exit(0);
      }
      a[i] = v;
    } else {
      i64 q = (v - rmin) / r;
      a[i] = a[i - 1] + 1 + q;
      if (i > 1) chmin(a[i], a[i - 1] * 2);
    }
    v -= a[i];
  }
  DEBUG(a);
  a.pop_front();
  cout << "YES\n";
  pprint(a);
}
