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

const i64 INF = 1e15;

i64 solve() {
  i64 n, k, s, t;
  cin >> n >> k >> s >> t;
  vector<pair<i64, i64>> car(n);
  REP(i, n) {
    i64 c, v;
    cin >> c >> v;
    car[i] = {c, v};  // price and tank capacity
  }
  const int k2 = k + 2;
  vector<i64> g(k2);  // gas station positions, distinct
  REP(i, k) cin >> g[i + 1];
  g[0] = 0;
  g[k + 1] = s;
  sort(ALL(g));
  vector<i64> d(k + 1);
  REP(i, k + 1) { d[i] = g[i + 1] - g[i]; }
  i64 dmax = *max_element(ALL(d));

  auto check = [&](i64 x) -> bool {
    if (x < dmax) return false;
    i64 tm = 0;
    REP(i, k + 1) {
      i64 rem = x - d[i];
      assert(rem >= 0);
      assert(d[i] >= 0);
      i64 ti = 2LL * d[i] - min(rem, d[i]);
      tm += ti;
      if (tm > t) return false;
    }
    return true;
  };

  i64 fv = -1, tv = 10LL * s;
  while (tv - fv > 1) {
    i64 mid = (tv + fv) / 2LL;
    (check(mid) ? tv : fv) = mid;
  }

  i64 ans = INF;  // min price
  REP(i, n) {
    if (car[i].second < tv) continue;
    chmin(ans, car[i].first);
  }

  return ans == INF ? -1LL : ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
