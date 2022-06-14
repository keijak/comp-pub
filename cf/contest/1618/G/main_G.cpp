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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
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
std::ostream &print_seq(const Container &seq, const char *sep = " ",
                        const char *ends = "\n", std::ostream &os = std::cout) {
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

auto solve() {
  const int n = in, m = in, Q = in; // <= 2*10^5
  vector<Int> A = in(n), B = in(m), K = in(Q);
  vector<pair<Int, bool>> C(n + m);
  REP(i, n) C[i] = {A[i], true};
  REP(j, m) C[n + j] = {B[j], false};
  sort(ALL(C));
  DUMP(C);
  vector<int> bcnt(n + m + 1);
  vector<Int> csum(n + m + 1);
  REP(i, n + m) {
    csum[i + 1] = csum[i] + C[i].first;
    bcnt[i + 1] = bcnt[i] + (C[i].second ? 0 : 1);
  }
  deque<pair<Int, int>> D(n + m - 1);
  REP(i, n + m - 1) {
    Int gap = C[i + 1].first - C[i].first;
    D[i] = {gap, i + 1};
  }
  sort(ALL(D));

  vector<pair<Int, int>> queries(Q);
  REP(i, Q) queries[i] = {K[i], i};
  sort(ALL(queries));

  vector<Int> ans(Q, -1);
  UnionFind uf(n + m);

  Int cost = accumulate(ALL(A), 0LL);
  DUMP(cost);
  REP(i, Q) {
    auto [k, qi] = queries[i];
    while (not D.empty() and D.front().first <= k) {
      auto [gap, x] = D.front();
      D.pop_front();
      int lsz = uf.size(x - 1);
      int rsz = uf.size(x);
      int lb = bcnt[x] - bcnt[x - lsz];
      int rb = bcnt[x + rsz] - bcnt[x];
      Int lcost = csum[x] - csum[x - lsz + lb];
      Int rcost = csum[x + rsz] - csum[x + rb];
      cost -= lcost + rcost;
      int sb = lb + rb;
      Int scost = csum[x + rsz] - csum[x - lsz + sb];
      cost += scost;
      uf.unite(x - 1, x);
    }
    ans[qi] = cost;
  }
  print_seq(ans, "\n");
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
