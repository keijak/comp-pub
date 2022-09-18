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

template<typename Monoid>
struct SWAGQueue {
  using T = typename Monoid::T;

  struct Node {
    T val, agg;
    Node(T val, T agg) : val(std::move(val)), agg(std::move(agg)) {}
  };
  std::stack<Node> front_stack, back_stack;

  SWAGQueue() = default;

  bool empty() const { return front_stack.empty() and back_stack.empty(); }

  int size() const { return front_stack.size() + back_stack.size(); }

  void push_back(const T &x) {
    if (back_stack.empty()) {
      back_stack.emplace(x, x);
    } else {
      back_stack.emplace(x, Monoid::op(back_stack.top().agg, x));
    }
  }

  void pop_front() {
    assert(!empty());
    if (front_stack.empty()) {
      front_stack.emplace(back_stack.top().val, back_stack.top().val);
      back_stack.pop();
      while (!back_stack.empty()) {
        T agg = Monoid::op(back_stack.top().val, front_stack.top().agg);
        front_stack.emplace(back_stack.top().val, std::move(agg));
        back_stack.pop();
      }
    }
    front_stack.pop();
  }

  T fold() const {
    if (empty()) return Monoid::id();
    if (front_stack.empty()) return back_stack.top().agg;
    if (back_stack.empty()) return front_stack.top().agg;
    return Monoid::op(front_stack.top().agg, back_stack.top().agg);
  }
};

struct MinOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

auto solve() {
  int n = in, m = in;
  map<int, int> md;
  Int base_sum = 0;
  REP(i, n) {
    Int a = in, b = in;
    base_sum += a;
    ++md[b - a];
  }
  vector<int> dp(m + 1, kBig<int>);
  dp[base_sum] = 0;
  for (auto [delta, dcnt]: md) {
    if (delta == 0) continue;
    auto cur = dp;
    int mod = abs(delta);
    vector<SWAGQueue<MinOp>> minq(mod);
    REP(jx, mod) {
      auto &q = minq[jx];
      for (int jy = 0;; ++jy) {
        int j = jy * mod + jx;
        if (j > m) break;
        int r = (delta < 0) ? m - j : j;
        q.push_back(cur[r] - jy);
        while (q.size() > dcnt + 1) q.pop_front();
        int minval = q.fold() + jy;
        dp[r] = minval;
      }
    }
  }
  REP(i, m + 1) {
    int x = dp[i];
    if (x == kBig<int>) x = -1;
    out(x);
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
