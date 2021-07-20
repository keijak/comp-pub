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

struct WeightedUnionFind {
  int n_;
  std::vector<int> parent_;
  std::vector<int> rank_;
  std::vector<i64> weight_; // diff from the parent_ node

  explicit WeightedUnionFind(int n)
      : n_(n), parent_(n, -1), rank_(n, 0), weight_(n, 0) {
    std::iota(parent_.begin(), parent_.end(), 0);
  }

  bool unite(int x, int y, i64 w) {
    w += weight(x);
    w -= weight(y);
    x = find(x), y = find(y);
    if (x == y) return false;
    if (rank_[x] < rank_[y]) {
      std::swap(x, y);
      w = -w;
    }
    if (rank_[x] == rank_[y]) ++rank_[x];
    parent_[y] = x;
    weight_[y] = w;
    return true;
  }

  int find(int v) {
    if (parent_[v] == v) return v;
    int r = find(parent_[v]);
    weight_[v] += weight_[parent_[v]];
    return parent_[v] = r;
  }

  bool same(int x, int y) { return find(x) == find(y); }

  // Returns the relative weight from the component root.
  i64 weight(int x) {
    find(x);
    return weight_[x];
  }

  // Returns the delta between two nodes.
  // Assumes x and y are in the same component.
  i64 diff(int x, int y) { return weight(y) - weight(x); }
};

auto solve() {
  INPUT(int, n, m);
  WeightedUnionFind uf(n);
  REP(i, m) {
    INPUT(int, l, r, d);
    --l, --r;
    if (uf.same(l, r)) {
      auto x = uf.diff(l, r);
      if (x != d) {
        return false;
      }
    } else {
      uf.unite(l, r, d);
    }
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "Yes" : "No");
  }
}
