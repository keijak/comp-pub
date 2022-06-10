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
    assert(std::cin >> x);
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) assert(std::cin >> x);
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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
  std::string unused;
  assert(not(std::cin >> unused));  // No input is left behind.
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
void dump_test_case(int t, int T) {
  if (T > 1) {
    std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
              << std::endl;
  }
}
#else
#define DUMP(...)
#define dump_test_case(...)
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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    int ub = std::upper_bound(values.begin(), values.end(), x) - values.begin();
    return ub - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

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

auto solve() {
  const int n = in, m = in, Q = in;
  vector<IntervalSet> buildings;
  REP(i, n) buildings.emplace_back(false);
  vector<Int> ys;
  REP(j, m) {
    Int a = in, b = in, c = in;
    --a;
    buildings[a].add_interval(b, c + 1);
    ys.push_back(b);
    ys.push_back(c);
  }
  const Int roof = Int(1e9) + 100;
  ys.push_back(roof);
  ys.push_back(0);
  Compressed<Int> ys_comp(ys);
  DUMP(ys_comp.values);
  const int P = ys_comp.size();
  vector<int> next_point(P);
  REP(i, P) next_point[i] = i;
  {
    vector<pair<int, int>> eranges;
    REP(i, n) {
      for (auto [l, r]: buildings[i]) {
        eranges.emplace_back(ys_comp.index(r - 1), ys_comp.index(l));
      }
    }
    sort(ALL(eranges), greater{});
    int cur = P - 1;
    for (auto [top, bottom]: eranges) {
      assert(top >= bottom);
      if (cur < 0) break;
      if (cur < bottom) continue;
      for (int j = min(cur, top); j >= bottom; --j) {
        chmax(next_point[j], top);
      }
      chmin(cur, bottom - 1);
    }
    DUMP(P, next_point);
  }
  SimpleFunctionalGraph<45> fg(P);
  REP(i, P) {
    fg.set_next(i, next_point[i]);
  }

  auto calc = [&](int x1, Int y1, int x2, Int y2) -> optional<Int> {
    if (y1 > y2) {
      swap(y1, y2);
      swap(x1, x2);
    }
    auto it1 = buildings[x1].find_interval(y1);
    auto it2 = buildings[x2].find_interval(y2);
    if (it1 != buildings[x1].end()) {
      y1 = it1->second - 1;
    }
    if (it2 != buildings[x2].end()) {
      y2 = it2->first;
    }
    if (y1 >= y2) {
      return (x1 == x2) ? 0 : 1;
    }
    const int start = ys_comp.lower_index(y1);
    const Int dest = y2;
    const int eventual = fg.go(start, 3 * m);
    if (ys_comp.value(eventual) < dest) {
      return nullopt;
    }
    assert(ys_comp.value(start) < dest);
    Int steps = fg.min_steps(start, [&](int y_id) -> bool {
      return ys_comp.value(y_id) >= dest;
    });
    DUMP(steps);
    return steps + 1;
  };

  REP(_, Q) {
    dump_test_case(_, Q);
    Int x1 = in, y1 = in, x2 = in, y2 = in;
    --x1, --x2;
    optional<Int> h_steps = calc(x1, y1, x2, y2);
    if (not h_steps.has_value()) {
      print(-1);
    } else {
      print(abs(y2 - y1) + *h_steps);
    }
  }
}

int main() {
  init_();
  solve();
  exit_();
}
