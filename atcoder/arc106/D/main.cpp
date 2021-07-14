#include <bits/stdc++.h>

#include <atcoder/all>
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
using mint = atcoder::modint998244353;
const int MOD = 998244353;

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

// C[n][k] = nCk
// Cretates a combination matrix using Pascal's triangle.
// Time and space complexity: Θ(n^2)
// Useful to avoid factorials causing integer overflow.
template <class T = mint>
vector<vector<T>> comb_table(int n) {
  vector<vector<T>> C(n + 1, vector<T>(n + 1, 0));
  for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  V<int> A(N);
  cin >> A;

  Factorials fs(K + 1);

  auto s = vector(K + 1, mint(0));
  REP(i, N) {
    mint p = 1;
    REP(k, K + 1) {
      s[k] += p;
      p *= A[i];
    }
  }
  REP(k, K + 1) { s[k] *= fs.ifact[k]; }

  auto d = vector(K + 1, mint(0));
  REP(i, N) {
    mint p = 1;
    REP(k, K + 1) {
      d[k] += p;
      p *= A[i] * 2;
    }
  }

  for (int x = 1; x <= K; ++x) {
    mint ans = 0;
    REP(k, x + 1) { ans += s[k] * s[x - k]; }
    ans *= fs.fact[x];
    ans -= d[x];
    ans /= 2;
    cout << ans.val() << '\n';
  }
}
