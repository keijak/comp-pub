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
  int x0, y0;
  int x, y;
  int rankx;
  int odd_cycle_count;
};

struct UndoableUnionFind {
  int n, sz;
  std::vector<int> parent_, size_;
  std::stack<Record> head_, tail_;
  int odd_cycle_count;

  explicit UndoableUnionFind(int n) : n(n), sz(n * 2), parent_(sz), size_(sz, 1) {
    for (int i = 0; i < sz; ++i) parent_[i] = i;
    odd_cycle_count = 0;
  }

  int other(int v) {
    return (v >= n) ? v - n : v + n;
  }

  bool unite(int x, int y, int i, bool reverse) {
    int x_ = x, y_ = y;
    x = find(x), y = find(y);
    if (size_[x] < size_[y]) std::swap(x, y);
    int ocdelta = 0, rankdelta = 0;
    if (x != y) {
      int oddx = same(x, other(x));
      int oddy = same(y, other(y));
      parent_[y] = x;
      rankdelta = size_[y];
      size_[x] += rankdelta;
      int oddx2 = same(x, other(x));
      ocdelta = oddx2 - oddx - oddy;
      odd_cycle_count += ocdelta;
    }
    if (reverse) {
      head_.push(Record{i, x_, y_, x, y, size_[x], odd_cycle_count});
    } else {
      tail_.push(Record{i, x_, y_, x, y, size_[x], odd_cycle_count});
    }
    return true;
  }

  void push(int u, int v, int i) {
    int u2 = n + u, v2 = n + v;
    unite(u, v2, i, false);
    unite(v, u2, i, false);
  }

  int find(int k) {
    if (parent_[k] == k) return k;
    return find(parent_[k]);  // no path compression
  }

  bool same(int x, int y) { return find(x) == find(y); }

  // Undoes one unite() call.
  void undo(int til) {
    while (true) {
      if (head_.empty() and not tail_.empty()) {
        vector<Record> records;
        while (not tail_.empty()) {
          auto record = tail_.top();
          tail_.pop();
          records.push_back(record);
          parent_[record.y] = record.y;
          size_[record.x] -= size_[record.y];
          odd_cycle_count = record.odd_cycle_count;
        }
        for (const auto &record: records) {
          unite(record.x0, record.y0, record.index, true);
        }
      }
      if (head_.empty()) break;
      const auto &record = head_.top();
      if (record.index > til) break;
      head_.pop();
      int x = find(record.x0);
      int y = find(record.y0);
      int oddx = same(record.x0, other(record.x0));
      parent_[y] = y;
      size_[record.x] -= size_[record.y];
      if (not same(record.x, record.y)) {
        int oddx2 = same(record.x, other(record.x));
        int oddy2 = same(record.y, other(record.y));
        odd_cycle_count += oddx2 + oddy2 - oddx;
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
    uf.push(u, v, i);
  };
  REP(i, m) {
    auto&[u, v] = edges[i];
    cin >> u >> v;
    --u, --v;
    push(i);
    DUMP(i, uf.odd_cycle_count, uf.parent_);
  }

  vector<int> llim(m + 1, -1);
  if (uf.odd_cycle_count == 0) {
    REP(i, m + 1) llim[i] = kBigVal<int>;
  } else {
    int l = 0;
    for (int r = 1; r <= m; ++r) {
      uf.undo(r - 1);
      while (l < r and uf.odd_cycle_count == 0) {
        push(m + l);
        l++;
      }
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
