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
  string pcolor;
  cin >> pcolor;
  const int n = ssize(pcolor);
  vector<pair<char, int>> runl;
  REP(i, n) {
    if (i > 0 and pcolor[i] == pcolor[i - 1]) {
      runl.back().second++;
    } else {
      runl.emplace_back(pcolor[i], 1);
    }
  }
  i64 ans = 0;
  vector<pair<char, int>> tmp;
  tmp.reserve(ssize(runl));
  while (ssize(runl) > 1) {
    int k = ssize(runl);
    int minc = min(runl.front().second, runl.back().second);
    for (int i = 1; i < k - 1; ++i) {
      int c = (runl[i].second + 1) / 2;
      chmin(minc, c);
    }
    ans += minc;
    DEBUG(minc, ans);
    REP(i, k) {
      if (i == 0 or i == k - 1) {
        runl[i].second -= minc;
      } else {
        runl[i].second -= minc * 2;
      }
    }
    tmp.clear();
    REP(i, k) {
      if (runl[i].second <= 0) continue;
      if (tmp.empty() or tmp.back().first != runl[i].first) {
        tmp.push_back(runl[i]);
      } else {
        tmp.back().second += runl[i].second;
      }
    }
    if (tmp == runl) break;
    swap(tmp, runl);
    DEBUG(runl);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
