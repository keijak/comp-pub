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

template <typename Monoid>
struct ReRooting {
  using T = typename Monoid::T;
  Monoid monoid;

  int n;                  // number of nodes
  vector<vector<int>> g;  // graph (tree)
  vector<T> sub;          // values for each subtree rooted at i.
  vector<T> full;         // values for each entire tree rooted at i
  int base_root;          // base root node where we start DFS

  explicit ReRooting(Monoid monoid, vector<vector<int>> g, int r = 0)
      : monoid(monoid),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const vector<T> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, monoid.id());
    return full;
  }

 private:
  T pull_up(int v, int par) {
    T res = monoid.id();
    for (int u : g[v]) {
      if (u == par) continue;
      res = monoid.op(res, pull_up(u, v));
    }
    sub[v] = monoid.build(v, res);
    return sub[v];
  }

  void push_down(int v, int par, T upper_sub) {
    int m = g[v].size();
    vector<T> cuml(m + 1, monoid.id()), cumr(m + 1, monoid.id());

    for (int i = 0; i < m; ++i) {
      int u = g[v][i];
      if (u == par) {
        cuml[i + 1] = monoid.op(cuml[i], upper_sub);
      } else {
        cuml[i + 1] = monoid.op(cuml[i], sub[u]);
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      int u = g[v][i];
      if (u == par) {
        cumr[i] = monoid.op(upper_sub, cumr[i + 1]);
      } else {
        cumr[i] = monoid.op(sub[u], cumr[i + 1]);
      }
    }

    full[v] = monoid.build(v, cuml[m]);

    for (int i = 0; i < m; ++i) {
      int u = g[v][i];
      if (u == par) continue;
      T next_upper_sub = monoid.build(v, monoid.op(cuml[i], cumr[i + 1]));
      push_down(u, v, move(next_upper_sub));
    }
  }
};

struct S {
  vector<int> color;

  explicit S(int n) : color(n) {}

  using T = int;

  T op(const T &x, const T &y) { return max(x, 0) + max(y, 0); }

  T id() { return 0; }

  T build(int node, T val) {
    int d = color[node] ? 1 : -1;
    return val + d;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  S s(n);
  REP(i, n) { cin >> s.color[i]; }

  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  ReRooting<S> rr(s, g);
  auto res = rr.run();
  print_seq(res);
}
