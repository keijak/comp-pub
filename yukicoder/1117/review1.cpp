#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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

const Int kBig = 1e16;

struct MaxOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return -kBig; }
};

auto solve() {
  int n = in, K = in, M = in;
  vector<Int> a = in(n);
  vector<Int> acc(n + 1);
  REP(i, n) acc[i + 1] = acc[i] + a[i];
  vector<Int> acc_minus(n + 1);
  REP(i, n + 1) acc_minus[i] = -acc[i];

  auto dp = vector(2, vector(n + 1, -kBig));
  dp[0][0] = 0;

  REP(k, 1, K + 1) {
    vector<Int> nv1(n + 1, -kBig), nv2(n + 1, -kBig);
    for (int l = 0; l <= n; ++l) {
      Int v = dp[0][l];
      if (v == -kBig) continue;
      nv1[l] = v - acc[l];
      nv2[l] = v - acc_minus[l];
    }
    SWAGQueue<MaxOp> rmq1, rmq2;
    REP(r, 1, n + 1) {
      rmq1.push_back(nv1[r - 1]);
      while (rmq1.size() > M) rmq1.pop_front();
      if (auto v = rmq1.fold(); v != -kBig) {
        chmax(dp[1][r], v + acc[r]);
      }
      rmq2.push_back(nv2[r - 1]);
      while (rmq2.size() > M) rmq2.pop_front();
      if (auto v = rmq2.fold(); v != -kBig) {
        chmax(dp[1][r], v + acc_minus[r]);
      }
    }
    swap(dp[0], dp[1]);
    dp[1].assign(n + 1, -kBig);
  }
  return dp[0][n];
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
