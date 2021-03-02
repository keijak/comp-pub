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

const i64 BMAX = int(1e9) + 5;

i64 f(const vector<i64> &a, const vector<i64> &b) {
  int n = a.size();
  int m = b.size();
  if (n == 0 or m == 0) return 0;
  DEBUG(a, b);
  map<i64, int> bm;
  REP(i, m) bm[b[i]] = i;
  vector<int> rcum(n + 1, 0);
  for (int i = n - 1; i >= 0; --i) {
    rcum[i] = bm.count(a[i]) + rcum[i + 1];
  }

  auto bit = bm.begin();
  while (bit != bm.end() and bit->first < a[0]) ++bit;
  i64 res = 0;
  REP(i, n) {
    i64 nex = (i + 1 < n) ? a[i + 1] : BMAX;
    for (; bit != bm.end() and bit->first < nex; ++bit) {
      i64 x = bit->first;
      int pos2 = bit->second;
      auto jt = bm.upper_bound(x - (i + 1));
      int pos1 = jt->second;
      chmax(res, pos2 - pos1 + 1 + rcum[i + 1]);
      // DEBUG(i, pos1, pos2, res);
    }
  }
  return res;
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<i64> a(n), b(m);
  cin >> a >> b;

  vector<i64> ar, br, al, bl;
  REP(i, n) {
    if (a[i] >= 0) {
      ar.push_back(a[i]);
    } else {
      al.push_back(-a[i]);
    }
  }
  REP(i, m) {
    if (b[i] >= 0) {
      br.push_back(b[i]);
    } else {
      bl.push_back(-b[i]);
    }
  }
  i64 ans = f(ar, br);
  DEBUG(ans);

  reverse(ALL(al));
  reverse(ALL(bl));
  ans += f(al, bl);
  DEBUG(ans);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) cout << solve() << "\n";
}
