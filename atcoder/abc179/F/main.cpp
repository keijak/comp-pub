#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
bool chmax(T &a, T b) {
  return a >= b ? false : ((a = std::move(b)), true);
}
template <class T>
bool chmin(T &a, T b) {
  return a <= b ? false : ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
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
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  int N, Q;
  cin >> N >> Q;
  map<int, int> sv, sh;
  sv[N] = N - 1;
  sh[N] = N - 1;

  i64 wcnt = 0;
  REP(i, Q) {
    int t, x;
    cin >> t >> x;
    x--;
    if (t == 1) {
      auto it = sv.lower_bound(x);
      int r = it->second;
      wcnt += max(r - 1, 0);
      sh[r] = min(sh.lower_bound(r)->second, x);
      DEBUG(x, r, wcnt);
      DEBUG(sh);
    } else {
      assert(t == 2);
      auto it = sh.lower_bound(x);
      int c = it->second;
      wcnt += max(c - 1, 0);
      sv[c] = min(sv.lower_bound(c)->second, x);
      DEBUG(x, c, wcnt);
      DEBUG(sv);
    }
  }
  DEBUG(wcnt);
  i64 ans = (N - 2) * (i64)(N - 2);
  ans -= wcnt;
  cout << ans << endl;
}
