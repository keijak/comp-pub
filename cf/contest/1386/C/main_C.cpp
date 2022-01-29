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
  int index;
  int u0, v0, u, u2, v, v2;
  int parent_u, parent_u2, parent_v, parent_v2;
  int odd_cycle_count;
  bool reverse;
};

struct UndoableUnionFind {
  int n, sz;
  std::vector<int> parent_;
  std::stack<Record> tail_;
  int odd_cycle_count;

  explicit UndoableUnionFind(int n) : n(n), sz(n * 2), parent_(sz, -1) {
    odd_cycle_count = 0;
  }

  int other(int v) {
    return (v >= n) ? v - n : v + n;
  }

  void unite(int x, int y) {
    x = find(x), y = find(y);
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    if (x != y) {
      parent_[x] += parent_[y];
      parent_[y] = x;
    }
  }

  void unite2(int u, int v, int i, bool reversed = false) {
    int u0 = u, v0 = v;
    int u2 = n + u, v2 = n + v;
    int saved_odd_cycle_count = odd_cycle_count;

    u = find(u), v2 = find(v2);
    v = find(v), u2 = find(u2);
    int saved_size_u = parent_[u];
    int saved_size_v2 = parent_[v2];
    int saved_size_v = parent_[v];
    int saved_size_u2 = parent_[u2];
    bool uodd = u == u2;
    bool vodd = v == v2;
    bool connected = u == v2;
    unite(u, v2);
    unite(v, u2);
    if (not connected) {
      odd_cycle_count -= uodd;
      odd_cycle_count -= vodd;
      odd_cycle_count += find(u) == find(u2);
    }
    tail_.push(Record{i, u0, v0, u, u2, v, v2, saved_size_u, saved_size_u2, saved_size_v, saved_size_v2,
                      saved_odd_cycle_count, reversed});
  }

  int find(int k) {
    if (parent_[k] < 0) return k;
    return find(parent_[k]);  // no path compression
  }

  bool same(int x, int y) { return find(x) == find(y); }

  // Undoes one unite() call.
  void undo(int til) {
    while (not tail_.empty()) {
      auto record = tail_.top();
      if (record.reverse and record.index > til) break;
      tail_.pop();
      parent_[record.u] = record.parent_u;
      parent_[record.u2] = record.parent_u2;
      parent_[record.v] = record.parent_v;
      parent_[record.v2] = record.parent_v2;
      odd_cycle_count = record.odd_cycle_count;
      if (record.reverse) continue;
      vector<Record> brecords = {record};
      vector<Record> arecords = {};

      while (not tail_.empty()) {
        auto r = tail_.top();
        tail_.pop();
        parent_[r.u] = r.parent_u;
        parent_[r.u2] = r.parent_u2;
        parent_[r.v] = r.parent_v;
        parent_[r.v2] = r.parent_v2;
        odd_cycle_count = r.odd_cycle_count;
        (r.reverse ? arecords : brecords).push_back(r);
        if (brecords.size() == arecords.size()) break;
      }
      bool reversed = tail_.empty();
      if (reversed) reverse(ALL(brecords));
      for (auto it = brecords.rbegin(); it != brecords.rend(); ++it) {
        unite2(it->u0, it->v0, it->index, reversed);
      }
      for (auto it = arecords.rbegin(); it != arecords.rend(); ++it) {
        unite2(it->u0, it->v0, it->index, true);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in, m = in, Q = in;
  vector<pair<int, int>> edges(m);
  UndoableUnionFind uf(n);

  auto push = [&](int i) {
    auto[u, v] = edges[i % m];
    uf.unite2(u, v, i);
  };
  REP(i, m) {
    auto&[u, v] = edges[i];
    cin >> u >> v;
    --u, --v;
    push(i);
    DUMP(i, uf.odd_cycle_count, uf.parent_);
  }

  vector<int> llim(m + 1, kBigVal<int>);
  if (uf.odd_cycle_count > 0) {
    llim[0] = 0;
    int l = 0;
    for (int r = 1; r <= m; ++r) {
      uf.undo(r - 1);
      while (l < r and uf.odd_cycle_count == 0) {
        push(m + l);
        l++;
      }
      assert(l <= r);
      llim[r] = l;
    }
  }
  DUMP(llim);

  REP(i, Q) {
    int l, r;
    cin >> l >> r;
    --l;
    print(l >= llim[r] ? "YES" : "NO");
  }
}
