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

struct Record {
  int x, y;
  int size_x;
  int odd_cycle_count;
};

struct UndoableUnionFind {
  int n_, sz_;
  std::vector<int> parent_, size_;
  std::stack<Record> history_;
  int odd_cycle_count_;

  explicit UndoableUnionFind(int n) : n_(n), sz_(n * 2), parent_(sz_, -1), size_(sz_, 1) {
    REP(v, sz_) parent_[v] = v;
    odd_cycle_count_ = 0;
  }

  int other(int v) {
    return (v < n_ ? v + n_ : v - n_);
  }
  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (size_[x] < size_[y]) std::swap(x, y);
    history_.push(Record{x, y, size_[x], odd_cycle_count_});
    int ox = same(x, other(x));
    int oy = same(y, other(y));
    size_[x] += size_[y];
    parent_[y] = x;
    int oxnew = same(x, other(x));
    odd_cycle_count_ += oxnew - ox - oy;
    return true;
  }

  void unite2(int u, int v) {
    int u2 = u + n_, v2 = v + n_;
    unite(u, v2);
    unite(u2, v);
  }

  int find(int k) {
    if (parent_[k] == k) return k;
    return find(parent_[k]);  // no path compression
  }

  bool same(int x, int y) { return find(x) == find(y); }

  void undo() {
    if (history_.empty()) return;
    auto record = history_.top();
    history_.pop();
    size_[record.x] = record.size_x;
    parent_[record.y] = record.y;
    odd_cycle_count_ = record.odd_cycle_count;
  }

  void rollback(int checkpoint) {
    while (int(history_.size()) > checkpoint) undo();
  }
};

struct Query {
  int idx;
  int l;
  int r;
};

struct Mo {
  static constexpr int B = 450;
  int n, m;
  vector<pair<int, int>> edges;
  vector<int> answer;
  vector<Query> queries;

  explicit Mo(int n, int m, int Q) : n(n), m(m), edges(m), answer(Q) {}

  void add_query(Query query) { queries.push_back(query); }

  void solve() {
    int q = int(queries.size());
    deque<int> ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / B;
      int bblock = queries[b].l / B;
      if (ablock != bblock) return ablock < bblock;
      return queries[a].r > queries[b].r;
    });
    UndoableUnionFind uf(n);
    for (int block = 0; block <= m / B; ++block) {
      int save0 = uf.history_.size();
      if (ord.empty()) break;
      auto q0 = queries[ord.front()];
      int bhead = q0.l / B * B;
      int r = m - 1;
      while (not ord.empty()) {
        auto q = queries[ord.front()];
        if (q.l / B != block) break;
        ord.pop_front();
        for (; r >= q.r; --r) {
          auto[u, v] = edges[r];
          uf.unite2(u, v);
        }
        int save1 = uf.history_.size();
        for (int i = bhead; i < q.l; ++i) {
          auto[u, v] = edges[i];
          uf.unite2(u, v);
        }
        answer[q.idx] = uf.odd_cycle_count_;
        uf.rollback(save1);
      }
      uf.rollback(save0);
      for (int i = bhead; i < min(bhead + B, m); ++i) {
        auto[u, v] = edges[i];
        uf.unite2(u, v);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in, m = in, Q = in;
  Mo mo(n, m, Q);
  for (auto &[u, v]: mo.edges) {
    cin >> u >> v;
    --u, --v;
  }
  REP(i, Q) {
    Query q{};
    q.idx = i;
    cin >> q.l >> q.r;
    --q.l;
    mo.add_query(q);
  }
  mo.solve();
  REP(i, Q) {
    print(mo.answer[i] ? "YES" : "NO");
  }
}
