#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;

// Partially Persistent UnionFind.
struct UnionFindWithTime {
  int n;
  mutable std::vector<int> parent_;  // positive: parent, negative: size
  std::vector<int> rank_;
  int num_roots_;
  int clock_;
  std::vector<int> parented_time_;
  std::vector<std::vector<std::pair<int, int>>> size_history_;

  explicit UnionFindWithTime(int sz)
      : n(sz),
        parent_(sz, -1),
        rank_(sz, 1),
        num_roots_(sz),
        clock_(0),
        parented_time_(sz, -1),
        size_history_(n, {{0, 1}}) {}

  // Returns current clock_.
  int unite(int x, int y) {
    ++clock_;
    x = find(x, clock_), y = find(y, clock_);
    if (x == y) return clock_;
    if (rank_[x] < rank_[y] or
        (rank_[x] == rank_[y] and -parent_[x] < -parent_[y])) {
      std::swap(x, y);
    }
    parent_[x] += parent_[y];
    parent_[y] = x;
    rank_[x] = std::max(rank_[x], rank_[y] + 1);
    parented_time_[y] = clock_;
    size_history_[x].emplace_back(clock_, -parent_[x]);
    --num_roots_;
    return clock_;
  }

  int find(int v, int time) const {
    if (parent_[v] < 0) return v;
    if (time < parented_time_[v]) return v;
    return find(parent_[v], time);
  }
  int find(int v) const { return find(v, clock_); }

  int size(int v, int time) const {
    int r = find(v, time);
    const auto &h = size_history_[r];
    auto it = std::lower_bound(h.begin(), h.end(), std::pair(time + 1, -1));
    return (--it)->second;
  }
  int size(int v) const { return -parent_[find(v)]; }

  bool same(int u, int v, int time) const {
    return find(u, time) == find(v, time);
  }
  bool same(int u, int v) const { return find(u) == find(v); }

  std::optional<int> united_time(int u, int v) {
    if (not same(u, v)) {
      return std::nullopt;
    }
    int fv = 0, tv = clock_;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      if (same(u, v, mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  }
};

auto solve() {
  int n = in, m = in;
  vector<tuple<int, int, int>> edges(m);
  REP(i, m) {
    int a = in, b = in, c = in;
    --a, --b;
    edges[i] = {c, a, b};
  }
  vector<int> ix(m);
  REP(i, m) ix[i] = i;
  sort(ALL(ix), [&](int i, int j) { return edges[i] < edges[j]; });

  UnionFindWithTime uf(n);
  Int total = 0;
  set<int> mste;
  vector<int> record(n + 1);
  REP(i, m) {
    int j = ix[i];
    auto[c, a, b] = edges[j];
    if (uf.same(a, b)) continue;
    DUMP(uf.clock_, j, a, b);
    uf.unite(a, b);
    total += c;
    mste.insert(j);
    record[uf.clock_] = c;
  }
  REP(i, m) {
    if (mste.count(i)) {
      print(total);
      continue;
    }
    auto[c, a, b] = edges[i];
    int t = uf.united_time(a, b).value();
    print(total - record[t] + c);
  }

}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  solve();
}
