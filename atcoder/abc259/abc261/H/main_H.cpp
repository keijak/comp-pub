// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
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

using namespace std;

struct Edge {
  int to;
  Int weight;
};
bool operator<(const Edge &e1, const Edge &e2) {
  return e1.to < e2.to;
}

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

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() {
  int n = in, m = in, v0 = in;
  --v0;
  vector<vector<Edge>> g(n);
  vector<set<Edge>> rg(n);
  REP(i, m) {
    int a = in, b = in, c = in;
    --a, --b;
    g[a].push_back({b, Int(c)});
    rg[b].insert({a, Int(c)});
  }
  auto dp = vector(2, vector(n, kBig<Int>));
  REP(v, n) dp[1][v] = -1;
  auto deg = vector(2, vector<int>(n));
  REP(v, n) {
    REP(i, 2) {
      deg[i][v] = ssize(g[v]);
    }
  }

  MinHeap<tuple<Int, int, int>> heap;
  REP(v, n) {
    REP(i, 2) {
      if (deg[i][v] == 0) {
        heap.push({0, v, i});
        dp[i][v] = 0;
      }
    }
  }
  while (not heap.empty()) {
    auto [dis, v, turn] = heap.top();
    heap.pop();
    if (dis != dp[turn][v]) continue;
    for (const Edge &e: rg[v]) {
      const int u = e.to;
      const Int du = dis + e.weight;
      const int tu = turn ^ 1;
      --deg[tu][u];
      if (tu == 0) {
        if (chmin(dp[tu][u], du)) {
          heap.push({dp[tu][u], u, tu});
        }
      } else {
        chmax(dp[tu][u], du);
        if (deg[tu][u] == 0) {
          heap.push({dp[tu][u], u, tu});
        }
      }
    }
  }

  Int ans = dp[0][v0];
  if (ans >= kBig<Int>) {
    out("INFINITY");
  } else {
    out(ans);
  }
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
