#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
//using Int = long long;
using Uint = unsigned long long;
using Real = long double;
#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::checked_int256_t;
// using Uint = boost::multiprecision::checked_uint256_t;
namespace multip = boost::multiprecision;

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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;
template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

auto solve() -> bool {
  vector<Int> a(6, 0), b(6, 0);
  REP(i, 5) cin >> a[i + 1];
  REP(i, 5) cin >> b[i + 1];
  {
    Int asum = 0, bsum = 0;
    for (int i = 1; i <= 5; ++i) {
      asum += a[i] * i;
      bsum += b[i] * i;
    }
    if (asum > bsum) return false;
  }
  for (int i = 1; i <= 5; ++i) {
    Int m = min(a[i], b[i]);
    a[i] -= m;
    b[i] -= m;
  }
  if (a[5] > 0) {
    return false;
  }
  {
    Int m = min(a[4], b[5]);
    a[4] -= m;
    b[5] -= m;
    b[1] += m;
    if (a[4] > 0) return false;
  }
  {
    Int m = min({a[2], a[3], b[5]});
    a[2] -= m;
    a[3] -= m;
    b[5] -= m;
  }
  {
    Int m = min(a[3], b[4]);
    a[3] -= m;
    b[4] -= m;
    b[1] += m;
  }
  {
    Int m = min(a[3], b[5]);
    a[3] -= m;
    b[5] -= m;
    b[2] += m;
    if (a[3] > 0) return false;
  }
  for (int i = 5; i >= 2; --i) {
    if (a[2] == 0) break;
    Int q = i / 2;
    Int m = min(a[2], b[i] * q);
    a[2] -= m;
  }
  if (a[2] > 0) {
    return false;
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
