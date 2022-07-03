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
template<int kNumBits = 60>
struct SimpleFunctionalGraph {
 private:
  // number of nodes.
  int size;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

  // Builds the transition table.
  void build_() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kNumBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
      }
    }
    build_done_ = true;
  }

 public:
  explicit SimpleFunctionalGraph(int n)
      : size(n), next_pos(kNumBits, std::vector<int>(n)), build_done_(false) {
    for (int d = 0; d < kNumBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Starting from `start`, `steps` times goes forward and returns
  // the end node.
  int go(int start, long long steps) {
    // steps >= 2^kNumBits is not supported.
    assert(steps < (1LL << kNumBits));
    build_();
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        cur_node = next_pos[d][cur_node];
      }
    }
    return cur_node;
  }

  template<class F>
  long long min_steps(int start, F pred) {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    build_();
    long long max_steps_false = 0;
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0; --d) {
      const int next_node = next_pos[d][cur_node];
      const bool overrun = pred(next_node);
      if (d == kNumBits - 1) {  // First time, the biggest jump.
        assert(overrun);        // kNumBits is too small!
      } else if (not overrun) {
        max_steps_false += 1LL << d;
        cur_node = next_node;
      }
    }
    return max_steps_false + 1;
  }
};
// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > T(1)) {
    auto mid = (truthy & falsy) + (truthy ^ falsy) / T(2);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

auto solve() {
  const int n = in, Q = in;
  const Int X = in;
  vector<Int> W = in(n);
  const Int wsum = accumulate(ALL(W), 0LL);

  vector<Int> W2;
  W2.reserve(n * 2 + 1);
  W2.push_back(0);
  REP(j, 2) {
    REP(i, n) W2.push_back(W[i]);
  }
  REP(i, n * 2) W2[i + 1] += W2[i];

  auto count_potato = [&](int i) -> Int {
    Int p = X / wsum;
    Int r = X % wsum;
    Int s = bisect((Int) n, -1LL, [&](Int j) -> bool {
      return W2[i + j] - W2[i] >= r;
    });
    return p * n + s;
  };

  SimpleFunctionalGraph<60> g(n);
  REP(i, n) {
    Int cnt = count_potato(i);
    int nex = (i + cnt) % n;
    g.set_next(i, nex);
  }

  REP(i, Q) {
    Int K = in;
    --K;
    int pos = g.go(0, K);
    out(count_potato(pos));
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
