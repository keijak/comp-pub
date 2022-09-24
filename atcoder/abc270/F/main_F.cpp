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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
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

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Event {
  int etype;
  Int cost;
  int x;
  int y;
};
bool operator>(const Event &x, const Event &y) {
  return x.cost > y.cost;
}

auto solve() {
  const int n = in, m = in;
  const vector<Int> X = in(n), Y = in(n);
  vector<pair<Int, int>> sx(n), sy(n);
  REP(i, n) {
    sx[i] = {X[i], i};
    sy[i] = {Y[i], i};
  }
  sort(ALL(sx));
  sort(ALL(sy));
  vector<tuple<Int, int, int>> edges(m);
  REP(i, m) {
    int a = in, b = in, z = in;
    --a, --b;
    edges[i] = {(Int) z, a, b};
  }
  const int xr = sx[0].second, yr = sy[0].second;
  Int ans = kBig<Int>;
  REP(iter, 4) {
    MinHeap<Event> heap;
    REP(i, m) {
      auto [z, a, b] = edges[i];
      heap.push({0, z, a, b});
    }
    const bool use_x = iter & 1;
    const bool use_y = iter & 2;
    Int cost = 0;
    if (use_x) {
      cost += sx[0].first;
      REP(i, 1, n) {
        heap.push({1, sx[i].first, sx[i].second, -1});
      }
    }
    if (use_y) {
      cost += sy[0].first;
      REP(i, 1, n) {
        heap.push({2, sy[i].first, sy[i].second, -1});
      }
    }
    UnionFind uf(n);
    int merge_cnt = 0;
    while (not heap.empty() and merge_cnt < n - 1) {
      auto ev = heap.top();
      heap.pop();
      if (ev.etype == 0) {
        if (uf.same(ev.x, ev.y)) continue;
        uf.unite(ev.x, ev.y);
        cost += ev.cost;
        ++merge_cnt;
      } else if (ev.etype == 1) {
        if (uf.same(ev.x, xr)) continue;
        uf.unite(ev.x, xr);
        cost += ev.cost;
        ++merge_cnt;
      } else {
        if (uf.same(ev.x, yr)) continue;
        uf.unite(ev.x, yr);
        cost += ev.cost;
        ++merge_cnt;
      }
    }
    if (merge_cnt == n - 1) {
      chmin(ans, cost);
    }
  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
