// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &output_seq(
    const Container &seq, const char *sep = " ", const char *ends = "\n",
    std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &output_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &output(const T &x) {
  return output_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &output(const T &head, Ts... tail) {
  return output_one(head, ' '), output(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

using namespace std;

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

void gen(int n) {
  vector<string> t(n, string(n, '.'));
  REP(i, n) {
    t[i][0] = t[i][1] = t[i][2] = '#';
  }

  auto to_coord = [&](int i) -> pair<int, int> {
    return {i / n, i % n};
  };
  auto to_id = [&](int x, int y) -> int {
    return x * n + y;
  };

  while (true) {
    string g;
    REP(i, n) {
      std::shuffle(t[i].begin(), t[i].end(), PRNG());
      g += t[i];
    }
    bool ok = true;

    REP(x, n) {
      int cnt = 0;
      REP(y, n) {
        if (t[y][x] == '#') ++cnt;
      }
      if (cnt != 3) {
        ok = false;
        break;
      }
    }
    if (not ok) continue;

    UnionFind uf(g.size() + 1);
    int kEmpty = g.size();

    REP(j, g.size()) {
      if (g[j] == '.') {
        uf.unite(j, kEmpty);
      } else {
        auto [x, y] = to_coord(j);
        if (x > 0) {
          int k = to_id(x - 1, y);
          if (g[k] == '#') uf.unite(j, k);
        }
        if (x < n - 1) {
          int k = to_id(x + 1, y);
          if (g[k] == '#') uf.unite(j, k);
        }
        if (y > 0) {
          int k = to_id(x, y - 1);
          if (g[k] == '#') uf.unite(j, k);
        }
        if (y < n - 1) {
          int k = to_id(x, y + 1);
          if (g[k] == '#') uf.unite(j, k);
        }
      }
    }
    int cnt = 0;
    REP(j, g.size() + 1) {
      if (uf.find(j) == j) ++cnt;
    }
    if (cnt == n + 1) {
      for (auto row: t) {
        output(row);
      }
      return;
    }
  }
}

int main() {
  init_();
  for (int i = 5; i <= 11; ++i) {
    cout << "=== " << i << " ===" << endl;
    gen(i);
  }
  cout << "Done!" << endl;
  exit_();
}
