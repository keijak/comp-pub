#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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

void solve() {
  int n, m;
  cin >> n >> m;
  vector<pair<int, int>> merges(m);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    merges[i] = {a, b};
  }
  int q;
  cin >> q;
  vector<pair<int, int>> queries(q);
  REP(i, q) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    queries[i] = {x, y};
  }

  vector<int> ans(q, -1);

  vector<int> fv(q, 0), tv(q, m + 1);
  REP(iter, 18) {
    map<int, vector<int>> check;
    REP(i, q) {
      if (tv[i] - fv[i] > 1) {
        int mid = (fv[i] + tv[i]) / 2;
        check[mid].push_back(i);
      }
    }
    if (check.empty()) break;

    UnionFind uf(n);
    REP(i, m) {
      auto [a, b] = merges[i];
      uf.unite(a, b);
      for (auto j : check[i + 1]) {
        auto [x, y] = queries[j];
        if (uf.same(x, y)) {
          tv[j] = i + 1;
        } else {
          fv[j] = i + 1;
        }
      }
    }
  }

  REP(i, q) {
    int ans = tv[i] == m + 1 ? -1 : tv[i];
    cout << ans << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
