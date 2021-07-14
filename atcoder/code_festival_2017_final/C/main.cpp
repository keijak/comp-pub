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

  int n;
  cin >> n;
  V<int> D(n);
  cin >> D;
  sort(ALL(D));

  auto closest = [](int d, const multiset<int> &ms) -> int {
    auto it = ms.lower_bound(d);
    int res = 1e9;
    if (it != ms.end()) {
      int x = *it - d;
      assert(x >= 0);
      chmin(res, min(x, 24 - x));
    }
    if (it != ms.begin()) {
      int x = d - (*--it);
      assert(x >= 0);
      chmin(res, min(x, 24 - x));
    }
    return res;
  };

  auto check = [&](int md) -> bool {
    multiset<int> pos;
    pos.insert(0);
    pos.insert(24);
    REP(i, n) {
      int d = closest(D[i], pos);
      if (d >= md) {
        pos.insert(D[i]);
      } else {
        int rd = (24 - D[i]) % 24;
        int d2 = closest(rd, pos);
        if (d2 >= md) {
          pos.insert(rd);
        } else {
          return false;
        }
      }
    }
    int p = -100;
    for (auto x : pos) {
      if (x - p < md) return false;
      p = x;
    }
    return true;
  };

  int ans = 0;
  for (int md = 0; md <= 12; ++md) {
    if (check(md)) ans = md;
  }
  cout << ans << endl;
}
