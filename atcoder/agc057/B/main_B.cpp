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

#include <boost/multiprecision/cpp_int.hpp>
namespace multip = boost::multiprecision;
using BigInt = multip::int128_t;
//using BigInt = multip::cpp_int;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() -> Int {
  Int n = in, X = in;
  vector<Int> a = in(n);

  vector<Int> b;
  REP(i, n) {
    if (X >= a[i]) continue;
    b.push_back(a[i]);
  }

  DUMP(a);
  DUMP(b);

  map<Int, Int> f;
  {
    REP(i, ssize(b)) {
      Int c = b[i];
      while (c % 2 == 0) {
        c /= 2;
      }
      auto it = f.find(c);
      if (it == f.end()) {
        f[c] = b[i];
      } else {
        chmax(it->second, b[i]);
      }
    }
  }
  DUMP(ssize(f), f);
  if (ssize(f) <= 1) return 0;

  return bisect(Int(1e9) + 100, -1LL, [&](Int width) -> bool {
    MinHeap<tuple<BigInt, BigInt>> heap;
    BigInt max_lo = -1;
    BigInt min_hi = kBig<Int>;

    for (const auto &[_, v]: f) {
      heap.push({v, v});
      chmax(max_lo, v);
      chmin(min_hi, v);
    }
    if (max_lo <= min_hi + width) return true;
    while (not heap.empty()) {
      auto [hi, lo] = heap.top();
      heap.pop();
      chmax(max_lo, 2 * lo);
      heap.push({2 * hi + X, 2 * lo});
      min_hi = get<0>(heap.top());
      if (max_lo <= min_hi + width) return true;
      if (hi >= (1ull << 33)) return false;
    }
    assert(false);
  });
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
