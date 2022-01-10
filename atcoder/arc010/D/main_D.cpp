#include <bits/stdc++.h>
#include <atcoder/scc>
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
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
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
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct Point {
  using T = long long;
  T x, y;
  int index;

  static T square_distance(T x1, T y1, T x2, T y2) {
    T dx = x1 - x2, dy = y1 - y2;
    return dx * dx + dy * dy;
  }
};

template<class P>
class KdTree2d {
  using T = typename P::T;
  using PItr = typename std::vector<P>::iterator;
  std::vector<P> points_;

  class NodePool;

  class Node {
    std::optional<Point> p;
    Node *left, *right;

   public:
    Node() : p{}, left(nullptr), right(nullptr) {}

    void init(PItr begin, PItr end, int depth, NodePool &pool) {
      if (begin == end) return;

      const int n = int(end - begin);
      auto mid = begin + (n >> 1);

      if (depth & 1) {
        std::nth_element(begin, mid, end,
                         [](const P &a, const P &b) { return a.y < b.y; });
      } else {
        std::nth_element(begin, mid, end,
                         [](const P &a, const P &b) { return a.x < b.x; });
      }
      p = *mid;
      if (begin != mid) {
        left = pool.get();
        left->init(begin, mid, depth + 1, pool);
      }
      if (std::next(mid) != end) {
        right = pool.get();
        right->init(std::next(mid), end, depth + 1, pool);
      }
    }

    // Returns the square distance and the pointer to the nearest neighbor.
    std::pair<T, P *> find_nearest(T x, T y, std::pair<T, P *> radius,
                                   int depth) {
      if (not p) return radius;

      T d = P::square_distance(x, y, p->x, p->y);
      if (radius.first > d) {
        radius = std::pair{d, &*p};
      }
      if (depth & 1) {
        if (left and y - p->y <= radius.first) {
          auto r = left->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
        if (right and p->y - y <= radius.first) {
          auto r = right->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
      } else {
        if (left and x - p->x <= radius.first) {
          auto r = left->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
        if (right and p->x - x <= radius.first) {
          auto r = right->find_nearest(x, y, radius, depth + 1);
          if (radius.first > r.first) radius = std::move(r);
        }
      }
      return radius;
    }
  };
  Node root_;

  class NodePool {
    std::vector<Node> nodes;
    size_t counter;

   public:
    explicit NodePool(size_t n) : nodes(n), counter(0) {}
    inline Node *get() { return &nodes[counter++]; }
  };
  NodePool pool_;

 public:
  explicit KdTree2d(std::vector<P> points)
      : points_(std::move(points)), pool_(points_.size() * 2) {
    root_.init(points_.begin(), points_.end(), 0, pool_);
  }

  std::pair<T, P *> find_nearest(T x, T y) {
    return root_.find_nearest(x, y, {std::numeric_limits<T>::max(), nullptr},
                              0);
  }
};

auto solve() -> int {
  const int n = in;
  vector<Point> friends(n);
  REP(i, n) {
    auto &f = friends[i];
    int x = in, y = in;
    f.x = x;
    f.y = y;
    f.index = i;
  }
  const int m = in;
  vector<Point> spies(m);
  REP(i, m) {
    auto &s = spies[i];
    int x = in, y = in;
    s.x = x;
    s.y = y;
    s.index = i;
  }
  if (m == 0) {
    return 1;
  }
  KdTree2d<Point> spy_tree(spies);
  vector<vector<int>> from(n);
  atcoder::scc_graph g(n);
  REP(i, n) {
    const auto &f = friends[i];
    const auto&[dist, p] = spy_tree.find_nearest(f.x, f.y);
    REP(j, n) {
      if (j == i) continue;
      if (Point::square_distance(friends[i].x, friends[i].y, friends[j].x, friends[j].y) < dist) {
        g.add_edge(i, j);
        from[j].push_back(i);
      }
    }
  }
  auto scc = g.scc();
  vector<int> comp(n);
  REP(i, scc.size()) {
    for (int v: scc[i]) comp[v] = i;
  }
  vector<int> indeg(scc.size());
  REP(v, n) {
    for (int u: from[v]) {
      if (comp[u] != comp[v]) indeg[comp[v]]++;
    }
  }
  int cnt = 0;
  REP(i, scc.size()) {
    if (indeg[i] == 0) ++cnt;
  }
  return cnt;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
