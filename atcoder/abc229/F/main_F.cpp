#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};

template <class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <class T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template <typename Container>
std::ostream &print_seq(const Container &seq, const char *sep = " ",
                        const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template <typename T>
    operator T() const {
      T x(n);
      for (auto &e : x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "backward.hpp"
#include "debug_dump.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr \
  if (false) cerr
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
  int n = in;
  vector<Int> a = in(n), b = in(n);
  Int asum = accumulate(ALL(a), 0LL);
  Int bsum = accumulate(ALL(b), 0LL);
  Int total = asum + bsum;
  DUMP(total, asum, bsum);
  auto dp = vector(n + 1, vector(2, vector(2, optional<Int>())));

  auto f = [&](auto &f, int i, bool wp, bool w1) -> Int {
    if (dp[i][wp][w1].has_value()) {
      return dp[i][wp][w1].value();
    }
    Int ret = 0;
    if (i == n) {
      if (wp) {
        if (not w1) chmax(ret, b[i - 1] + a[i - 1]);
        chmax(ret, a[i - 1]);
      } else {
        if (w1) chmax(ret, b[i - 1]);
        chmax(ret, 0);
      }
      dp[i][wp][w1] = ret;
      // DUMP(i, wp, w1, aw, ret);
      return ret;
    }

    auto sb = f(f, i + 1, false, w1);
    auto sw = f(f, i + 1, true, w1);
    if (wp) {
      chmax(ret, sb + b[i - 1] + a[i - 1]);
      chmax(ret, sw + a[i - 1]);
    } else {
      chmax(ret, sw + b[i - 1]);
      chmax(ret, sb);
    }
    dp[i][wp][w1] = ret;
    // DUMP(i, wp, w1, aw, ret);
    return ret;
  };
  DUMP(f(f, 1, true, true));

  return total - max(f(f, 1, true, true), f(f, 1, false, false));
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;  // in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
