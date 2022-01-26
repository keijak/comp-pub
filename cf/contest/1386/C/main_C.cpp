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
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "YES" : "NO") << endc;
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
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
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

struct UndoableUnionFind {
  std::vector<int> parent_, size_;
  std::stack<std::tuple<int, int, int>> lstack_, rstack_;

  explicit UndoableUnionFind(int sz) : parent_(sz, -1), size_(sz, 1) {
    REP(v, sz) parent_[v] = v;
  }

  bool unite(int x, int y, bool rpush, int ei) {
    x = find(x), y = find(y);
    if (x == y) {
      (rpush ? rstack_ : lstack_).emplace(x, x, ei);
      return false;
    }
    if (size_[x] < size_[y]) {
      std::swap(x, y);
    }
    (rpush ? rstack_ : lstack_).emplace(y, x, ei);
    size_[x] += size_[y];
    parent_[y] = x;
    return true;
  }

  int find(int k) {
    if (parent_[k] == k) return k;
    return find(parent_[k]);  // no path compression
  }

  bool same(int x, int y) { return find(x) == find(y); }

  void lpop(int til) {
    while (not lstack_.empty()) {
      auto[y, x, ei] = lstack_.top();
      if (ei < til) break;
      lstack_.pop();
      if (x != y) {
        size_[x] -= size_[y];
        parent_[y] = y;
      }
    }
  }

};

struct Query {
  int idx;
  int l;
  int r;
};

struct Task {
  int n, m;
  vector<pair<int, int>> edges;
  UndoableUnionFind uf;
  vector<int> answer;
  int l_, r_;

  Task(int n, int m, int Q) : n(n), m(m), edges(m), uf(n * 2), answer(Q) {}

  void init() {
    for (int i = 0; i < m; ++i) {
      connect_(i, false);
    }
    l_ = r_ = 0;
    DUMP(l_, r_, uf.parent_, uf.size_);
  }

  void connect_(int i, bool rpush) {
    const auto &e = edges[i];
    auto[u, v] = e;
    int u2 = n + u, v2 = n + v;
    uf.unite(u, v2, rpush, i);
    uf.unite(u2, v, rpush, i);
//    uf.unite(u, v, rpush);
  }

  void add_left(int l) {
    uf.lpop(l);
    for (int i = r_; i < m; ++i) {
      connect_(i, false);
    }
    --l_;
    DUMP(l_, r_, uf.parent_, uf.size_, odd_cycle());
  }
  void add_right(int r) {
    uf.lpop(0);
    for (int i = 0; i < m; ++i) {
      if (l_ <= i and i <= r) continue;
      connect_(i, false);
    }
    ++r_;
    DUMP(l_, r_, uf.parent_, uf.size_, odd_cycle());
  }
  void erase_left(int l) {
    connect_(l, false);
    l_++;
    DUMP(l_, r_, uf.parent_, uf.size_, odd_cycle());
  }
  void erase_right(int r) {
    connect_(r, false);
    --r_;
    DUMP(l_, r_, uf.parent_, uf.size_, odd_cycle());
  }

  // Processes one query on the current state.
  void process(const Query &q) {
    // Fill the answer of the query.
    answer[q.idx] = odd_cycle();
  }
  bool odd_cycle() {
    REP(v, n) {
      int v2 = n + v;
      if (uf.same(v, v2)) {
        return true;
      }
    }
    return false;
  }
};

struct Mo {
  static constexpr int B = 450;
  int n;
  vector<Query> queries;

  explicit Mo(int n) : n(n) {}

  void add_query(Query query) { queries.push_back(query); }

  void solve(Task &task) const {
    int q = int(queries.size());
    vector<int> ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / B;
      int bblock = queries[b].l / B;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries[a].r > queries[b].r
                          : queries[a].r < queries[b].r;
    });
    int l = 0, r = 0;
    for (auto idx: ord) {
      while (l > queries[idx].l) task.add_left(--l);
      while (r < queries[idx].r) task.add_right(r++);
      while (l < queries[idx].l) task.erase_left(l++);
      while (r > queries[idx].r) task.erase_right(--r);
      task.process(queries[idx]);
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in, m = in, Q = in;
  Task task(n, m, Q);
  for (auto &[u, v]: task.edges) {
    cin >> u >> v;
    --u, --v;
  }
  task.init();

  Mo mo(m);
  REP(i, Q) {
    Query q{};
    q.idx = i;
    cin >> q.l >> q.r;
    --q.l;
    mo.add_query(q);
  }
  mo.solve(task);
  REP(i, Q) {
    print(task.answer[i] ? "YES" : "NO");
  }
}
