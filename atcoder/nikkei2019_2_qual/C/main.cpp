#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  std::set<int> roots_;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1) {
    for (int i = 0; i < sz; ++i) roots_.insert(i);
  }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    roots_.erase(y);
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  const std::set<int> &roots() const { return roots_; }
};

auto solve() -> bool {
  const int n = in;
  vector<i64> a = in(n), b = in(n);
  {
    vector<int> d(n);
    REP(i, n) d[i] = i;
    sort(ALL(d), [&](int i, int j) { return b[i] < b[j]; });
    auto ta = a, tb = b;
    REP(i, n) {
      a[i] = ta[d[i]];
      b[i] = tb[d[i]];
    }
  }
  vector<i64> sa = a, sb = b;
  sort(ALL(sa));
  sort(ALL(sb));
  REP(i, n) {
    if (sa[i] > sb[i]) return false;
  }

  vector<int> p(n);
  REP(i, n) p[i] = i;
  sort(ALL(p), [&](int i, int j) { return a[i] < a[j]; });

  REP(i, n - 1) {
    if (a[p[i + 1]] <= sb[i]) return true;
  }

  UnionFind uf(n);
  REP(i, n) uf.unite(i, p[i]);
  return ssize(uf.roots()) > 1;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
