#define NDEBUG
#include<vector>
#include<bitset>
#include <iostream>
#include <sstream>
#include <cassert>
using namespace std;

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

constexpr int L = 200005;
bitset<L> visiting;

auto solve() -> void {
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  vector<int> in_cycle(n, -1), root(n);
  REP(v, n) root[v] = v;

  vector<int> seq;

  auto finish = [&](int u) -> void {
    while (not seq.empty()) {
      int x = seq.back();
      in_cycle[x] = 0;
      root[x] = u;
      seq.pop_back();
    }
  };

  bool detected = false;

  auto dfs = Rec([&](auto &self, int v, int p) -> bool {
    if (in_cycle[v] != -1) {
      finish(root[v]);
      return true;
    }
    if (not detected) {
      if (visiting[v]) {
        detected = true;
        while (not seq.empty()) {
          int x = seq.back();
          in_cycle[x] = 1;
          seq.pop_back();
          if (x == v) break;
        }
        finish(v);
        return true;
      }
      visiting.set(v);
    }
    seq.push_back(v);
    for (int u: g[v]) {
      if (u == p) continue;
      if (self(u, v)) return true;
    }
    seq.pop_back();
    return false;
  });

  REP(v, n) {
    if (g[v].size() != 1) continue;
//    vector<int> seq_;
//    seq.swap(seq_);
    dfs(v, -1);
  }

  const int Q = in;
  REP(qi, Q) {
    int x = in, y = in;
    --x, --y;
    out(root[x] == root[y]);
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
