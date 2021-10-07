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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

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

struct VersatileInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      static_assert(std::numeric_limits<T>::max() != T());  // max must be defined
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

using namespace std;
using Real = long double;
using P = complex<Real>;

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

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

auto solve() {
  const int n = in, m = in;
  vector<P> towers(n);
  REP(i, n) {
    int x = in, y = in;
    P p((Real) x, (Real) y);
    towers[i] = p;
  }
  vector<pair<P, Real>> roads(m);
  REP(i, m) {
    int x = in, y = in, r = in;
    P p((Real) x, (Real) y);
    roads[i] = pair{p, (Real) r};
  }
  DUMP(towers);
  DUMP(roads);
  Real ans = kBig;

  vector<tuple<Real, int, int>> edges;
  REP(i, m) {
    auto[ci, ri] = roads[i];
    REP(j, i) {
      auto[cj, rj] = roads[j];
      Real d = abs(ci - cj);
      Real mi = min(ri, rj);
      Real ma = max(ri, rj);
      Real e;
      if (d > ri + rj) {
        e = d - ri - rj;
      } else if (d + mi < ma) {
        e = ma - (d + mi);
      } else {
        e = 0.0;
      }
      edges.emplace_back(e, i, j);
    }
  }
  REP(i, n) {
    const auto &tower = towers[i];
    REP(j, n) {
      if (j == i) continue;
      const auto &other = towers[j];
      edges.emplace_back(abs(tower - other), m + i, m + j);
    }
  }
  REP(i, n) {
    const auto &tower = towers[i];
    REP(j, m) {
      const auto &center = roads[j].first;
      const auto &rr = roads[j].second;
      Real d = abs(tower - center);
      edges.emplace_back(abs(rr - d), j, m + i);
    }
  }
  sort(ALL(edges));

  REP(bits, 1 << m) {
    Real cost = 0;
    UnionFind uf(m + n);
    for (auto[e, i, j]: edges) {
      if (i < m and not has_bit(bits, i)) continue;
      if (j < m and not has_bit(bits, j)) continue;
      if (uf.same(i, j)) continue;
      uf.unite(i, j);
      cost += e;
    }
    chmin(ans, cost);
  }
  return ans;
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
