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

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<pair<int, int>> f2s(n), s2f(m);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    if (x > y) swap(x, y);
    f2s[i] = {x, y};
  }
  REP(i, m) {
    int x, y;
    cin >> x >> y;
    if (x > y) swap(x, y);
    s2f[i] = {x, y};
  }
  set<int> ans1, ans2;
  bool ambiguous = false;
  REP(i, n) {
    set<int> tmp;
    REP(j, m) {
      map<int, int> s;
      s[f2s[i].first]++;
      s[f2s[i].second]++;
      s[s2f[j].first]++;
      s[s2f[j].second]++;
      if (s.size() == 3) {
        for (auto [x, y] : s) {
          if (y == 2) {
            tmp.insert(x);
          }
        }
      }
    }
    if (ssize(tmp) > 1) ambiguous = true;
    ans1.insert(ALL(tmp));
  }
  REP(j, m) {
    set<int> tmp;
    REP(i, n) {
      map<int, int> s;
      s[f2s[i].first]++;
      s[f2s[i].second]++;
      s[s2f[j].first]++;
      s[s2f[j].second]++;
      if (s.size() == 3) {
        for (auto [x, y] : s) {
          if (y == 2) {
            tmp.insert(x);
          }
        }
      }
    }
    if (ssize(tmp) > 1) ambiguous = true;
    ans2.insert(ALL(tmp));
  }
  if (ambiguous) return -1;
  if (ssize(ans1) == 1 and ssize(ans2) == 1 and
      (*ans1.begin() == *ans2.begin())) {
    return *ans1.begin();
  }
  return 0;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
