#include <bits/stdc++.h>

#include <atcoder/modint>
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

using mint = atcoder::modint1000000007;
constexpr int MOD = 1000000007;

template <class T = mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(size_t n) : fact(n + 1), ifact(n + 1) {
    assert(n > 0 and n < MOD);
    fact[0] = 1;
    for (size_t i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (size_t i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination (nCk)
  T C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }

  // Permutation (nPk)
  T P(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[n - k];
  }
};

using namespace std;
ostream &operator<<(ostream &os, const mint &m) { return os << m.val(); }

mint solve() {
  int n;
  cin >> n;
  V<int> X(n);
  cin >> X;
  const int m = n - 1;
  Factorials<mint> fs(n);
  V<mint> D(m);
  REP(i, m) { D[i] = X[i + 1] - X[i]; }
  mint d = X[m] - X[0];

  mint ans = 0;
  for (int i = 1; i <= m; ++i) {
    mint r = d * fs.fact[m - i] * fs.C(m, i) * fs.fact[i - 1];
    ans += r;
    d -= D[i - 1];
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve().val() << endl;
}
