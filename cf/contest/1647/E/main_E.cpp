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

std::vector<Int> divisors(Int n) {
  std::vector<Int> res;
  for (Int k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    Int q = n / k;
    if (q != k) res.push_back(q);
  }
  std::sort(res.begin(), res.end());
  return res;
}

template<int kNumBits = 60>
struct SimpleFunctionalGraph {
 public:
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

auto solve() {
  int n = in;
  vector<int> P = in(n);
  vector<Int> a = in(n);
  for (auto &x: P)--x;
  for (auto &x: a)--x;
  vector<int> pf(n);
  map<int, int> inipos;
  REP(i, n) {
    ++pf[P[i]];
  }
  set<Int> missing;
  REP(i, n) { missing.insert(i); }
  REP(i, n) {
    Int x = a[i];
    if (x < n) {
      missing.erase(x);
    }
  }

  int repl = 0;
  REP(i, n) if (pf[i] == 0)++repl;
  DUMP(repl);
  assert(repl >= 1);
  Int amax = *max_element(ALL(a)) + 1;
  Int total_supplied = amax - n;
  Int lessons = total_supplied / repl;
  DUMP(repl, total_supplied, lessons);

  SimpleFunctionalGraph fg(n);
  REP(i, n) {
    fg.set_next(i, P[i]);
  }

  vector<Int> b(n, -1);
  if (lessons == 0) {
    b = a;
    for (auto &x: b) ++x;
    return b;
  }

  vector<int> filled(n, 0);
  vector<optional<int>> lb(n);
  REP(i, n) {
    int to = fg.go(i, lessons);
    Int x = a[to];
    if (x >= n) continue;
    if (filled[x]) {
      lb[i] = x + 1;
    } else {
      b[i] = x;
      filled[x] = true;
    }
  }
  REP(j, n) {
    if (b[j] != -1) continue;
    assert(lb[j].has_value());
    auto it = missing.lower_bound(lb[j].value());
    assert(it != missing.end());
    b[j] = *it;
    missing.erase(it);
  }

  for (auto &x: b) ++x;
  return b;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print_seq(ans);
  }
}
