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

auto solve() -> bool {
  const int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  const int K = in;
  vector<optional<int>> num(n);
  int root = -1;
  REP(i, K) {
    int v = in, p = in;
    --v;
    num[v] = p;
    root = v;
  }
  assert(root != -1);

  vector<int> parity(n, -1);
  Rec([&](auto self, int v, int p, int pari) -> void {
    parity[v] = pari;
    for (int u: g[v]) {
      if (u == p) continue;
      self(u, v, pari ^ 1);
    }
  })(root, -1, num[root].value() % 2);

  REP(v, n) {
    if (num[v].has_value() and parity[v] != num[v].value() % 2) {
      return false;
    }
  }

  vector<int> lo(n, -kBig<int>), hi(n, kBig<int>);
  bool ok = Rec([&](auto self, int v, int p) -> bool {
    for (int u: g[v]) {
      if (u == p) continue;
      if (not self(u, v)) return false;
      chmax(lo[v], lo[u] - 1);
      chmin(hi[v], hi[u] + 1);
    }
    if (lo[v] > hi[v]) return false;
    if (num[v]) {
      if (num[v].value() < lo[v] or num[v].value() > hi[v]) {
        return false;
      }
      lo[v] = hi[v] = num[v].value();
    }
    return true;
  })(root, -1);
  if (not ok) return false;

  DUMP(lo);
  DUMP(hi);
  ok = Rec([&](auto self, int v, int p) -> bool {
    if (not num[v]) {
      for (int d: {-1, +1}) {
        int val = num[p].value() + d;
        if (lo[v] <= val and val <= hi[v] and val % 2 == parity[v]) {
          num[v] = val;
          break;
        }
      }
      if (not num[v]) return false;
    }
    for (int u: g[v]) {
      if (u == p) continue;
      if (not self(u, v)) return false;
    }
    return true;
  })(root, -1);
  if (not ok) return false;
  REP(v, n) {
    if (not num[v]) return false;
  }
  out("Yes");
  REP(v, n) {
    out(num[v].value());
  }
  return true;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    bool res = (solve());
    if (not res) {
      out("No");
    }
  }
  exit_();
}
