#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
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

const int MOD = 998'244'353;
using Mint = atcoder::modint998244353;

template <class T = Mint>  // Usually used over ModInt.
struct Factorials {
  // factorials and inverse factorials.
  vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n < (int)MOD);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
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

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  V<int> H(2 * N);
  cin >> H;
  map<int, int> counts;
  REP(i, 2 * N) { ++counts[H[i]]; }
  Factorials fs(2 * N + 1);
  V<Mint> fact2(N + 1);  // = (2*k-1)!!
  {
    fact2[0] = fact2[1] = 1;
    for (int i = 2; i <= N; ++i) {
      fact2[i] = fact2[i - 1] * (2 * i - 1);
    }
  }

  int num_groups = counts.size();
  deque<V<Mint>> bad_counts(num_groups);
  {
    int i = 0;
    for (auto [_, k] : counts) {
      int m = k / 2 + 1;
      bad_counts[i].resize(m);
      bad_counts[i][0] = 1;
      for (int j = 1; j < m; ++j) {
        bad_counts[i][j] = fs.C(k, 2 * j) * fact2[j];
      }
      ++i;
    }
  }

  // Merge
  while (bad_counts.size() > 1) {
    auto x = move(bad_counts.front());
    bad_counts.pop_front();
    auto y = move(bad_counts.front());
    bad_counts.pop_front();
    auto z = atcoder::convolution<>(move(x), move(y));
    bad_counts.push_back(move(z));
  }

  Mint ans = 0;
  for (int i = 0; i < SIZE(bad_counts[0]); ++i) {
    const Mint &x = bad_counts[0][i];
    if (i % 2 == 0) {
      ans += x * fact2[N - i];
    } else {
      ans -= x * fact2[N - i];
    }
  }
  cout << ans.val() << endl;
}
