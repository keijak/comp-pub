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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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

i64 solve() {
  i64 n, m;
  cin >> n >> m;

  int ph = 1, pm = 1;
  i64 maxh = n - 1, maxm = m - 1;
  string maxhs, maxms;
  while (true) {
    char ch = (maxh % 7) + '0';
    maxhs.push_back(ch);
    maxh /= 7;
    if (maxh == 0) break;
    ph++;
  }
  while (true) {
    char ch = (maxm % 7) + '0';
    maxms.push_back(ch);
    maxm /= 7;
    if (maxm == 0) break;
    pm++;
  }
  reverse(ALL(maxhs));
  reverse(ALL(maxms));
  DEBUG(ph, pm);
  DEBUG(maxhs, maxms);
  if (ph + pm >= 8) return 0;
  const int W = ph + pm;

  V<int> choose(7);
  REP(i, W) { choose[6 - i] = 1; }
  string num = "0123456";
  int count = 0;
  do {
    string s;
    REP(i, 7) {
      if (choose[i]) s.push_back(num[i]);
    }
    do {
      auto hs = s.substr(0, ph);
      auto ms = s.substr(ph);
      if (hs <= maxhs and ms <= maxms) ++count;
    } while (next_permutation(ALL(s)));
  } while (next_permutation(ALL(choose)));
  return count;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve() << '\n';
}
