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

void solve() {
  int h;
  cin >> h;
  vector<int> a(h + 1);
  cin >> a;
  assert(a[0] == 1);
  int n = 0;  // num nodes
  for (int x : a) n += x;

  vector<int> par1(n, -2), par2(n, -2);
  par1[0] = par2[0] = -1;

  auto amb = [&](auto self, int i, int pl, int pr, bool dirty) -> bool {
    if (i == h + 1) return dirty;
    int x = a[i];
    if (x > 1 and (pr - pl) > 1) {
      REP(j, x) {
        par1[pr + j] = pl;
        par2[pr + j] = pl;
      }
      par2[pr] = pl + 1;
      dirty = true;
    } else if (x == 1) {
      par1[pr] = pl;
      par2[pr] = pl;
    } else {
      REP(j, x) {
        par1[pr + j] = pl;
        par2[pr + j] = pl;
      }
    }
    return self(self, i + 1, pr, pr + x, dirty);
  };

  if (not amb(amb, 1, 0, 1, false)) {
    cout << "perfect\n";
    return;
  }
  cout << "ambiguous\n";
  for (auto &x : par1) ++x;
  for (auto &x : par2) ++x;
  print_seq(par1);
  print_seq(par2);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
