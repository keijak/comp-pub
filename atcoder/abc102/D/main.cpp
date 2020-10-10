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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  V<i64> A(n);
  cin >> A;
  V<i64> cum(n + 1);
  REP(i, n) { cum[i + 1] = cum[i] + A[i]; }

  i64 ans = 1e18;
  for (int i = 1; i < n; ++i) {
    i64 lsum = cum[i], rsum = cum[n] - cum[i];
    i64 lt = (lsum + 1) / 2;
    i64 rt = lsum + (rsum + 1) / 2;
    auto mit = cum.begin() + i;
    auto lit = lower_bound(cum.begin(), mit, lt);
    V<i64> lp, rp;
    if (lit != mit) {
      lp.push_back(*lit);
    }
    if (lit != cum.begin()) {
      --lit;
      if (lit != cum.begin()) lp.push_back(*lit);
    }
    auto rit = lower_bound(mit, cum.end(), rt);
    if (rit != cum.end()) {
      rp.push_back(*rit - *mit);
    }
    if (rit != mit) {
      --rit;
      if (rit != mit) rp.push_back(*rit - *mit);
    }
    for (auto lv : lp) {
      for (auto rv : rp) {
        array<i64, 4> v = {lv, *mit - lv, rv, cum[n] - *mit - rv};
        i64 diff = *max_element(ALL(v)) - *min_element(ALL(v));
        chmin(ans, diff);
        DEBUG(i, v, diff, ans);
      }
    }
  }
  cout << ans << endl;
}
