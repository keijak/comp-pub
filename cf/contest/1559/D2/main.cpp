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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
type __VA_ARGS__;      \
read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1), num_roots(sz) {}

  optional<int> unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return nullopt;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    --num_roots;
    return y;
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

auto solve() -> vector<pair<int, int>> {
  INPUT(int, n, m1, m2);
  UnionFind uf1(n), uf2(n);
  REP(i, m1) {
    INPUT(int, u, v);
    --u, --v;
    uf1.unite(u, v);
  }
  REP(i, m2) {
    INPUT(int, u, v);
    --u, --v;
    uf2.unite(u, v);
  }
  auto rv1 = uf1.roots(), rv2 = uf2.roots();
  set<int> r1(ALL(rv1)), r2(ALL(rv2));

  vector<pair<int, int>> ans;
  auto unite = [&](int u, int v) {
    ans.push_back({u + 1, v + 1});

    auto z1 = uf1.unite(u, v);
    assert(z1.has_value());
    assert(r1.count(z1.value()));
    r1.erase(z1.value());

    auto z2 = uf2.unite(u, v);
    assert(z2.has_value());
    assert(r2.count(z2.value()));
    r2.erase(z2.value());
  };

  while (ssize(r1) >= 2 and ssize(r2) >= 2) {
    auto it1 = r1.begin();
    int u1 = *it1;
    int v1 = *++it1;
    auto it2 = r2.begin();
    int u2 = *it2;
    int v2 = *++it2;

    if (not uf2.same(u1, v1)) {
      unite(u1, v1);
      continue;
    }

    if (not uf1.same(u2, v2)) {
      unite(u2, v2);
      continue;
    }

    bool ok = false;
    for (int x : {u2, v2}) {
      if (uf2.same(x, u1)) continue;
      for (int y : {u1, v1}) {
        if (uf1.same(y, x)) continue;
        unite(x, y);
        ok = true;
        break;
      }
      if (ok) break;
    }
    assert(ok);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  print(ans.size());
  if (not ans.empty()) {
    for (auto[u, v] : ans) {
      print(u, v);
    }
  }
}
