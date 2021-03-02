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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    --num_roots;
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < n; ++i) {
      if (parent[i] < 0) res.push_back(i);
    }
    return res;
  }
};

struct Min {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

template <typename Abelian>  // Abelian Monoid
struct AggregatableUnionFind : public UnionFind {
  using T = typename Abelian::T;
  std::vector<T> agg_data;

  explicit AggregatableUnionFind(std::vector<T> data)
      : UnionFind(data.size()), agg_data(std::move(data)) {}

  bool unite(int x, int y) {
    int rx = this->find(x), ry = this->find(y);
    if (not UnionFind::unite(x, y)) return false;
    int r = this->find(x);
    agg_data[r] = Abelian::op(agg_data[rx], agg_data[ry]);
    return true;
  }

  const T &agg(int x) const { return agg_data[this->find(x)]; }
};

void solve() {
  int n;
  cin >> n;
  vector a(n, vector(n, 0LL));
  REP(i, n) { REP(j, n) cin >> a[i][j]; }

  vector<i64> salary;
  salary.reserve(n * 2);
  salary.resize(n, 0LL);
  REP(i, n) { salary[i] = a[i][i]; }  // leaf salary
  DEBUG(salary);

  map<pair<i64, int>, int> bossid;
  vector<vector<int>> ancestor(n);
  REP(i, n) { ancestor[i].push_back(i); }
  map<int, int> parent;

  deque<tuple<i64, int, int>> dq;
  REP(i, n) REP(j, i + 1, n) { dq.emplace_back(a[i][j], i, j); }
  sort(ALL(dq));

  vector<int> eid(n);
  REP(i, n) eid[i] = i;
  AggregatableUnionFind<Min> uf(eid);

  while (not dq.empty()) {
    auto [sal, i, j] = dq.front();
    dq.pop_front();
    assert(i < j);

    int ri = uf.agg(i);
    int rj = uf.agg(j);
    int r = min(ri, rj);

    pair<i64, int> bkey = {sal, r};
    auto it = bossid.find(bkey);
    if (it == bossid.end()) {
      int bid = salary.size();
      bossid[bkey] = bid;
      int p = ancestor[ri].back();
      if (salary[p] < sal) {
        parent[p] = bid;
      }
      if (ri != rj) {
        int p = ancestor[rj].back();
        if (salary[p] < sal) parent[p] = bid;
      }
      ancestor[r].push_back(bid);
      salary.push_back(sal);
    } else {
      int bid = it->second;
      for (int rx : {ri, rj}) {
        int p = ancestor[rx].back();
        if (salary[p] < sal) {
          parent[p] = bid;
        }
      }
    }
    uf.unite(i, j);
  }

  int k = salary.size();
  cout << k << "\n";
  print_seq(salary);
  int root = -1;
  REP(i, n) { assert(parent.find(i) != parent.end()); }
  REP(i, n, k) {
    if (parent.find(i) == parent.end()) {
      root = i;
      break;
    }
  }
  cout << root + 1 << "\n";
  DEBUG(parent);
  // assert(ssize(parent) == k - 1);
  for (auto [v, u] : parent) {
    cout << v + 1 << " " << u + 1 << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
