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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    assert(std::cin >> x);
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) assert(std::cin >> x);
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

inline void init_test_case(int t, int T) {
#ifdef MY_DEBUG
  if (T > 1) {
    std::cerr << "\033[35m=== case " << t << "/" << T << " ===\033[0m"
              << std::endl;
  }
#endif
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;
struct SparseSet {
  static constexpr int kMaxSize = int(1e5) + 10;
  static constexpr int kMaxValue = int(1e5) + 10;

  int size_ = 0;
  std::array<int, kMaxSize> dense_;
  std::array<int, kMaxValue> sparse_;

  // O(1)
  bool contains(int x) const {
    // May access uninitialized memory, but safe.
    const size_t i = sparse_[x];
    return i < (size_t) size_ and dense_[i] == x;
  }

  // O(1)
  void insert(int x) {
    if (contains(x)) return;
    dense_[size_] = x;
    sparse_[x] = size_;
    ++size_;
  }

  // O(1)
  void erase(int x) {
    const int i = sparse_[x];
    if (i < 0 or i >= size_ or dense_[i] != x) {
      return;  // not a member
    }
    auto y = dense_[size_ - 1];
    dense_[i] = y;
    sparse_[y] = i;
    --size_;
  }

  // O(1)
  inline void clear() { size_ = 0; }
  inline int size() const { return size_; }
  inline bool empty() const { return size_ == 0; }

  // O(size)
  template<class F>
  void for_each(F func) {
    for (int i = 0; i < size_; ++i) {
      func(dense_[i]);
    }
  }

  // For silencing warnings on accessing uninitialized memory.
  void safe_init() {
    dense_.fill(0);
    sparse_.fill(0);
  }
};

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

struct Seg {
  int c;
  Int l;
  Int r;
};

auto solve() {
  const int n = in;
  UnionFind uf(n);
  vector<Seg> segs(n);
  vector<tuple<Int, bool, int>> events;
  REP(i, n) {
    int c = in;
    Int l = in, r = in;
    segs[i] = Seg{c, l, r};
    events.emplace_back(l, false, i);
    events.emplace_back(r, true, i);
  }
  sort(ALL(events));
  array<SparseSet, 2> rs;
  array<optional<int>, 2> rt;

  for (const auto &ev: events) {
    Int x;
    bool ending;
    int i;
    tie(x, ending, i) = ev;
    const auto &si = segs[i];
    const int me = si.c;
    const int other = me ^ 1;
    if (ending) {
      if (rt[me].has_value() and *rt[me] == i) {
        rs[other].insert(rt[other].value());
        rt[me] = nullopt;
        rt[other] = nullopt;
      } else {
        rs[me].erase(i);
      }
    } else {
      if (rt[me].has_value() and rt[other].has_value()) {
        uf.unite(i, *rt[other]);
        if (segs[*rt[me]].r < si.r) {
          rt[me] = i;
        }
      } else if (not rs[other].empty()) {
        int v = -1;
        Int rmax = -1;
        rs[other].for_each([&](int j) {
          auto [cj, lj, rj] = segs[j];
          uf.unite(i, j);
          if (chmax(rmax, rj)) {
            v = j;
          }
        });
        rs[other].clear();
        rt[other] = v;
        rt[me] = i;
      } else {
        rs[me].insert(i);
      }
    }
  }
  int cnt = 0;
  REP(i, n) {
    if (uf.find(i) == i) ++cnt;
  }
  print(cnt);
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    init_test_case(t, T);
    (solve());
  }
  exit_();
}
