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

auto solve() {
  auto g = vector(4, vector(4, 0));
  int gcnt = 0;
  auto pos2id = [&](int i, int j) {
    return i * 4 + j;
  };
  auto id2pos = [&](int id) {
    int r = id / 4;
    int c = id % 4;
    return pair{r, c};
  };

  unsigned gbits = 0;

  REP(i, 4)
    REP(j, 4) {
      g[i][j] = in;
      if (g[i][j]) {
        ++gcnt;
        int x = pos2id(i, j);
        gbits |= 1 << x;
      }
    }
  DUMP(g);

  i64 ans = 0;
  REP(bits, 1, 1 << 16) {
    if ((gbits & bits) != gbits) continue;
    auto b = vector(6, vector(6, 0));
    int bcnt = 0;
    vector<int> bs;
    REP(i, 16) {
      if (bits >> i & 1) {
        auto[r, c] = id2pos(i);
        b[r + 1][c + 1] = 1;
        ++bcnt;
        bs.push_back(i);
      }
    }
    assert(not bs.empty());
//    int acnt = 0;
//    REP(i, 4) REP(j, 4) {
//        if (b[i+1][j+1] and g[i][j]) ++acnt;
//      }
    UnionFind uf(36);
    REP(i, 6) REP(j, 6) {
        if (i + 1 < 6 and b[i][j] == b[i + 1][j]) {
          int x = i * 6 + j;//pos2id(i, j);
          int y = (i + 1) * 6 + j;//pos2id(i + 1, j);
          uf.unite(x, y);
        }
        if (j + 1 < 6 and b[i][j] == b[i][j + 1]) {
          int x = i * 6 + j;//pos2id(i, j);
          int y = i * 6 + (j + 1);//pos2id(i, j + 1);
          uf.unite(x, y);
        }
      }

    bool ok = (bcnt == 16) or (bcnt < 16 and uf.roots().size() == 2);
//    REP(i, 1, bs.size()) {
//      if (not uf.same(bs[0], bs[i])) {
//        ok = false;
//        break;
//      }
//    }
    if (ok) {
      ++ans;
//      cerr << "===" << endl;
//      REP(i, 6) {
//        print_seq(b[i], " ", "\n", cerr);
//      }
//      cerr << "###" << endl;
    }
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
