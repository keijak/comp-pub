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

// A set of disjoint half-open intervals [l, r).
// (mapping l to r)
class IntervalSet : public std::map<Int, Int> {
 private:
  // If true, automatically merges [l, c) and [c, r).
  // Otherwise only merges intervals when they have non-empty overlap.
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

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

auto solve() {
  const int n = in;
  const Int L = in, W = in;
  vector<Int> A = in(n);
  IntervalSet iset(true);
  REP(i, n) {
    iset.add_interval(A[i], A[i] + W);
  }
  iset.add_interval(L, L + 10);
  Int ans = 0;
  Int p = 0;
  while (p < L) {
    auto jt = iset.find_interval(p);
    if (jt != iset.end()) {
      p = jt->second;
    }
    if (p >= L) break;
    {
      auto it = iset.lower_bound(p);
      assert (it != iset.end());
      Int x = it->first - p;
      Int y = it->second;
      Int k = ceil_div<Int>(x, W);
      ans += k;
      iset.add_interval(p, p + k * W);
      p = y;
    }
  }
  print(ans);
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
