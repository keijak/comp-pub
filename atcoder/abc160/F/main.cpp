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

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template <class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n > 0 and n < T::mod());
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

using namespace std;

vector<int> solve() {
  int n;
  cin >> n;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }

  vector<int> sizes(n, 0);
  auto dfs_sz = [&](auto self, int v, int p) -> int {
    int s = 1;
    for (auto w : g[v]) {
      if (w == p) continue;
      s += self(self, w, v);
    }
    return sizes[v] = s;
  };
  dfs_sz(dfs_sz, 0, -1);
  DEBUG(sizes);

  Factorials fs(n);
  vector<Mint> sub(n, 0);
  auto dfs_sub = [&](auto self, int v, int p) -> Mint {
    Mint res = 1;
    int m = sizes[v] - 1;
    for (auto w : g[v]) {
      if (w == p) continue;
      int wn = sizes[w];
      res *= fs.C(m, wn);
      m -= wn;
      res *= self(self, w, v);
    }
    sub[v] = res;
    return res;
  };
  dfs_sub(dfs_sub, 0, -1);
  DEBUG(sub);

  vector<int> ans(n, 0);
  auto dfs_ans = [&](auto self, int v, int p, Mint psub) -> void {
    Mint prod_subs = 1, prod_facts = 1;
    for (auto w : g[v]) {
      if (w == p) continue;
      int wn = sizes[w];
      prod_subs *= sub[w];
      prod_facts *= fs.fact[wn];
    }
    DEBUG(v, p, psub, prod_subs, prod_facts);
    int pn = n - sizes[v];
    prod_subs *= psub;
    prod_facts *= fs.fact[pn];
    Mint res = fs.fact[n - 1] * prod_subs / prod_facts;
    ans[v] = res.val();

    for (auto w : g[v]) {
      if (w == p) continue;
      int wn = sizes[w];
      int m = n - wn - 1;
      Mint psub2 = fs.fact[m] * (prod_subs / sub[w]) / prod_facts * fs.fact[wn];
      self(self, w, v, psub2);
    }
  };
  dfs_ans(dfs_ans, 0, -1, 1);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  print_seq(solve(), "\n");
}
