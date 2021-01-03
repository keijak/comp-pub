#include <cassert>
#include <iostream>
#include <string>
#include <vector>

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

int chromatic_number(const vector<vector<bool>> &g) {
  int N = (int)g.size();
  vector<int> es(N);
  for (int i = 0; i < g.size(); i++) {
    for (int j = 0; j < g.size(); j++) {
      es[i] |= g[i][j] << j;
    }
  }
  int ret = N;
  for (int d : {7, 11, 21}) {
    int mod = 1e9 + d;
    vector<int> ind(1 << N), aux(1 << N, 1);
    ind[0] = 1;
    for (int S = 1; S < 1 << N; S++) {
      int u = __builtin_ctz(S);
      ind[S] = ind[S ^ (1 << u)] + ind[(S ^ (1 << u)) & ~es[u]];
    }
    for (int i = 1; i < ret; i++) {
      int64_t all = 0;
      for (int j = 0; j < 1 << N; j++) {
        int S = j ^ (j >> 1);
        aux[S] = (1LL * aux[S] * ind[S]) % mod;
        all += j & 1 ? aux[S] : mod - aux[S];
      }
      if (all % mod) ret = i;
    }
  }
  return ret;
}

bool compatible(const string &s1, const string &s2) {
  int m = ssize(s1);
  REP(i, m) {
    if (s1[i] == '*') continue;
    if (s2[i] == '*') continue;
    if (s1[i] == s2[i]) continue;
    return false;
  }
  return true;
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<string> patterns(n);
  REP(i, n) { cin >> patterns[i]; }
  vector g(n, vector(n, false));
  REP(i, n) {
    REP(j, i) {
      if (not compatible(patterns[i], patterns[j])) {
        g[i][j] = g[j][i] = true;
      }
    }
  }
  return chromatic_number(g);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
