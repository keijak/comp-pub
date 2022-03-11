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

class IntervalSet : public std::map<Int, Int> {
 private:
  // If true, automatically merges [l, c) and [c, r).
  bool merge_adjacent;

 public:
  explicit IntervalSet(bool merge_adjacent = true)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r) which contains p if available.
  // Otherwise returns this->end().
  std::map<Int, Int>::iterator find_interval(Int p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second > p) return it;
    }
    return end();
  }

  // Inserts interval [l, r)
  void add_interval(Int l, Int r) {
    auto itl = upper_bound(l), itr = lower_bound(r + merge_adjacent);
    if (itl != begin()) {
      --itl;
      if (itl->second + merge_adjacent <= l) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      for (auto it = itl; it != itr;) {
        it = erase(it);
      }
    }
    (*this)[l] = r;
  }

  // Removes interval [l, r)
  void remove_interval(Int l, Int r) {
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l) ++itl;
    }
    if (itl == itr) return;
    Int tl = std::min(l, itl->first);
    Int tr = std::max(r, std::prev(itr)->second);
    for (auto it = itl; it != itr;) {
      it = erase(it);
    }
    if (tl < l) {
      (*this)[tl] = l;
    }
    if (r < tr) {
      (*this)[r] = tr;
    }
  }

  // Are p and q in the same interval?
  bool same(Int p, Int q) {
    const auto it = find_interval(p);
    return (it != end()) and (it->first <= q) and (q < it->second);
  }

  // Non-negative minimum excluded value.
  Int mex() {
    const auto it = find_interval(0);
    return (it != end()) ? it->second : 0;
  }
};

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

auto solve() {
  int n = in;
  vector<int> a = in(n);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int x = in, y = in;
    --x, --y;
    g[x].push_back(y);
    g[y].push_back(x);
  }

  IntervalSet iset(true);
  vector<int> counter(n + 1);

  int ans = 0;

  auto dfs = Rec([&](auto &dfs, int v, int p) -> void {
    if (++counter[a[v]] == 1) {
      iset.add_interval(a[v], a[v] + 1);
      chmax(ans, iset.mex());
    }
    for (auto u: g[v]) {
      if (u == p) continue;
      dfs(u, v);
    }
    if (--counter[a[v]] == 0) {
      iset.remove_interval(a[v], a[v] + 1);
      chmax(ans, iset.mex());
    }
  });
  dfs(0, -1);
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
