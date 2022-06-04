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

inline void init_test_case(int t, int T) {
#ifdef MY_DEBUG
  if (T > 1) {
    std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
              << std::endl;
  }
#endif
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

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
  int n = in;
  vector<int> D = in(n);
  if (n > 24) {
    return 0;
  }
  vector<int> cnt(13);
  cnt[0] = 1;
  REP(i, n) {
    if (D[i] == 0) return 0;
    cnt[D[i]]++;
  }
  REP(i, 1, 12) {
    if (cnt[i] >= 3) return 0;
  }
  if (cnt[0] >= 2 or cnt[12] >= 2) return 0;

  int ans = 0;
  vector<int> d;
  REP(bits, 1 << 11) {
    d.clear();
    if (cnt[0]) d.push_back(0);
    if (cnt[12]) d.push_back(12);
    REP(i, 1, 12) {
      if (cnt[i] == 2) {
        d.push_back(i);
        d.push_back(24 - i);
      }
    }

    REP(i, 11) {
      if (cnt[i + 1] != 1) continue;
      if (has_bit(bits, i)) {
        d.push_back(i + 1);
      } else {
        d.push_back(24 - (i + 1));
      }
    }
    assert(ssize(d) == n + 1);
    sort(ALL(d));
    int mindiff = kBigVal<int>;
    REP(i, n) {
      chmin(mindiff, d[i + 1] - d[i]);
    }
    chmin(mindiff, 24 - d[n]);
    chmax(ans, mindiff);
  }
  return ans;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    init_test_case(t, T);
    print(solve());
  }
  exit_();
}
