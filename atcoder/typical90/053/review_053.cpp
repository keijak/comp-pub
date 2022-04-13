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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class F, class T = std::invoke_result_t<F, Int>>
Int find_max_golden_section_search(Int low, Int high, F f) {
  static_assert(std::is_invocable_v<F, Int>);
  std::greater<T> compare;

  --low;             // Make it an open interval: (low, high).
  Int l = 1, r = 1;  // Left and right offsets from `low`.
  while (l + r < high - low) {
    l += r;
    std::swap(l, r);
  }
  auto lval = f(low + l), rval = f(low + r);
  while (l < r) {
    r -= l;
    std::swap(l, r);
    if (compare(lval, rval)) {
      rval = std::move(lval);
      lval = f(low + l);
    } else {
      low += r;
      if (low + r < high) {
        lval = std::move(rval);
        rval = f(low + r);
      } else {
        r -= l;
        std::swap(l, r);
        lval = f(low + l);
      }
    }
  }
  return low + 1;
}

auto solve() {
  int n;
  cin >> n;
  vector<optional<int>> a(n + 1);
  int opt_x = find_max_golden_section_search(1, n + 1, [&](int x) {
    if (a[x].has_value()) return a[x].value();
    cout << "? " << x << endl;
    int res;
    cin >> res;
    a[x] = res;
    return res;
  });
  cout << "! " << a[opt_x].value() << endl;
}

int main() {
  int T;
  cin >> T;
  REP(t, T) { (solve()); }
  exit_();
}
