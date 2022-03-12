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

const int MOD = 1000000007;
long long power(long long a, long long n) {
  long long res = 1;
  while (n > 0) {
    if (n & 1) res = res * a % MOD;
    a = a * a % MOD;
    n >>= 1;
  }
  return res;
}

int chromatic_number(const vector<vector<int>> &G) {
  int n = (int)G.size();
  vector<int> neighbor(n, 0);
  for (int i = 0; i < n; ++i) {
    int S = (1 << i);
    for (int j = 0; j < n; ++j)
      if (G[i][j]) S |= (1 << j);
    neighbor[i] = S;
  }

  // I[S] := #. of inndepndet subset of S
  vector<int> I(1 << n);
  I[0] = 1;
  for (int S = 1; S < (1 << n); ++S) {
    int v = __builtin_ctz(S);
    I[S] = I[S & ~(1 << v)] + I[S & ~neighbor[v]];
  }
  int low = 0, high = n;
  while (high - low > 1) {
    int mid = (low + high) >> 1;

    // g[S] := #. of "k independent sets" which cover S just
    // f[S] := #. of "k independent sets" which consist of subseta of S
    // then
    //   f[S] = sum_{T in S} g(T)
    //   g[S] = sum_{T in S} (-1)^(|S|-|T|)f[T]
    long long g = 0;
    for (int S = 0; S < (1 << n); ++S) {
      if ((n - __builtin_popcount(S)) & 1)
        g -= power(I[S], mid);
      else
        g += power(I[S], mid);
      g = (g % MOD + MOD) % MOD;
    }
    if (g != 0)
      high = mid;
    else
      low = mid;
  }
  return high;
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector g(n, vector(n, 1));

  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a][b] = g[b][a] = false;
  }
  int ans = chromatic_number(g);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
