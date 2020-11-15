#include <bits/stdc++.h>
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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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

struct UnionFind {
  mutable std::vector<int> par;  // positive: parent, negative: size
  int num_roots;
  V<map<int, int>> counter;

  explicit UnionFind(V<int> classes)
      : par(classes.size(), -1),
        num_roots(ssize(classes)),
        counter(ssize(classes)) {
    REP(i, classes.size()) { ++counter[i][classes[i]]; }
  }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (par[x] > par[y]) std::swap(x, y);
    par[x] += par[y];
    par[y] = x;
    --num_roots;

    // x is parent
    auto &m1 = counter[x];
    auto &m2 = counter[y];
    if (m1.size() > m2.size()) {
      for (auto [k, v] : m2) {
        m1[k] += v;
      }
    } else {
      for (auto [k, v] : m1) {
        m2[k] += v;
      }
      swap(counter[x], counter[y]);
    }
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
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<int> C(N);
  REP(i, N) {
    cin >> C[i];
    --C[i];
  }
  UnionFind uf(move(C));

  REP(i, Q) {
    int typ;
    cin >> typ;
    int a, b;
    cin >> a >> b;
    if (typ == 1) {
      uf.unite(a - 1, b - 1);
    } else {
      int r = uf.find(a - 1);
      cout << uf.counter[r][b - 1] << '\n';
    }
  }

  // solve();
}
