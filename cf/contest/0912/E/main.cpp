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
template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

using Int = __uint128_t;
const Int BIG = Int(1e18) + 5;

i64 solve() {
  int n;
  cin >> n;
  vector<int> p(n);
  cin >> p;  // ascending order
  i64 k;
  cin >> k;

  array<vector<int>, 2> pp;
  REP(i, n) { pp[i & 1].push_back(p[i]); }

  array<set<u64>, 2> sp;
  auto f = [&](auto rec, int i, Int x, int j) -> void {
    if (i == ssize(pp[j])) return;
    Int pval = pp[j][i];
    while (x < BIG) {
      sp[j].insert(x);
      rec(rec, i + 1, x, j);
      x *= pval;
    }
  };
  f(f, 0, 1, 0);
  f(f, 0, 1, 1);

  vector<u64> vp0(ALL(sp[0])), vp1(ALL(sp[1]));
  if (vp1.empty()) vp1.push_back(1);
  DEBUG(ssize(vp0), ssize(vp1));

  auto smaller = [&](Int val) -> i64 {
    int j = ssize(vp1) - 1;
    i64 count = 0;
    REP(i, ssize(vp0)) {
      if (vp0[i] >= val) break;
      while (j >= 0 and Int(vp0[i]) * vp1[j] >= val) --j;
      if (j < 0) break;
      count += j + 1;
    }
    return count;
  };

  Int tv = 0, fv = BIG;
  while (fv - tv > 1) {
    Int mid = (fv + tv) / 2;
    i64 x = smaller(mid);
    if (x < k) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  return tv;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}