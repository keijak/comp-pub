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

template <typename Monoid>
struct ReRooting {
  using T = typename Monoid::T;

  int n;                  // number of nodes
  vector<vector<int>> g;  // graph (tree)
  vector<T> sub;          // values for each subtree rooted at i.
  vector<T> full;         // values for each entire tree rooted at i
  int base_root;          // base root node where we start DFS

  explicit ReRooting(vector<vector<int>> g, int r = 0)
      : n((int)g.size()), g(move(g)), sub(n), full(n), base_root(r) {}

  const vector<T> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, Monoid::id());
    return full;
  }

 private:
  T pull_up(int v, int par) {
    T res = Monoid::id();
    for (int u : g[v]) {
      if (u == par) continue;
      res = Monoid::op(res, pull_up(u, v));
    }
    sub[v] = Monoid::build(res);
    return sub[v];
  }

  void push_down(int v, int par, T upper_sub) {
    int m = g[v].size();
    vector<T> cuml(m + 1, Monoid::id()), cumr(m + 1, Monoid::id());

    for (int i = 0; i < m; ++i) {
      int u = g[v][i];
      if (u == par) {
        cuml[i + 1] = Monoid::op(cuml[i], upper_sub);
      } else {
        cuml[i + 1] = Monoid::op(cuml[i], sub[u]);
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      int u = g[v][i];
      if (u == par) {
        cumr[i] = Monoid::op(upper_sub, cumr[i + 1]);
      } else {
        cumr[i] = Monoid::op(sub[u], cumr[i + 1]);
      }
    }

    full[v] = Monoid::build(cuml[m]);

    for (int i = 0; i < m; ++i) {
      int u = g[v][i];
      if (u == par) continue;
      T next_upper_sub = Monoid::build(Monoid::op(cuml[i], cumr[i + 1]));
      push_down(u, v, move(next_upper_sub));
    }
  }
};

#include <atcoder/modint>
using Mint = atcoder::modint;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

struct S {
  using T = Mint;

  static T op(const T &x, const T &y) { return x * y; }

  static T id() { return 1; }

  static T build(T val) { return val + 1; }
};

void solve() {
  int n, m;
  cin >> n >> m;
  Mint::set_mod(m);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    g[x].push_back(y);
    g[y].push_back(x);
  }
  ReRooting<S> rr(g);
  auto res = rr.run();
  REP(i, n) { cout << (res[i] - 1).val() << '\n'; }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
