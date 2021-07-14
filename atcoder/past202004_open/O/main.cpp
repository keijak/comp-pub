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

struct UnionFind {
  int n;
  mutable std::vector<int> par;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), par(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (par[x] > par[y]) std::swap(x, y);
    par[x] += par[y];
    par[y] = x;
    --num_roots;
    return true;
  }

  int find(int k) const {
    if (par[k] < 0) return k;
    return par[k] = find(par[k]);
  }

  int size(int k) const { return -par[find(k)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < (int)par.size(); ++i) {
      if (par[i] < 0) res.push_back(i);
    }
    return res;
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<tuple<i64, int, int>> edges(m);
  map<pair<int, int>, int> edge_id;
  REP(i, m) {
    int a, b, c;
    cin >> a >> b >> c;
    --a, --b;
    if (a > b) swap(a, b);
    edges[i] = {c, a, b};
    edge_id[{a, b}] = edge_id[{b, a}] = i;
  }
  auto sorted_edges = edges;
  sort(ALL(sorted_edges));

  vector<int> fv(m, 0), tv(m, m);
  i64 opt_cost = 0;
  while (true) {
    map<int, vector<int>> checks;
    REP(i, m) {
      if (tv[i] - fv[i] > 1) {
        int mid = (tv[i] + fv[i]) / 2;
        checks[mid].push_back(i);
      }
    }
    if (checks.empty()) break;

    UnionFind uf(n);
    opt_cost = 0;
    REP(i, m) {
      for (auto j : checks[i]) {
        auto [dcost, u, v] = edges[j];
        if (uf.same(u, v)) {
          tv[j] = i;
        } else {
          fv[j] = i;
        }
      }
      auto [c, a, b] = sorted_edges[i];
      if (uf.same(a, b)) continue;
      uf.unite(a, b);
      opt_cost += c;
    }
  }

  REP(i, m) {
    auto [xc, xa, xb] = edges[i];
    auto [yc, ya, yb] = sorted_edges[tv[i] - 1];
    cout << opt_cost - yc + xc << '\n';
  }
}
