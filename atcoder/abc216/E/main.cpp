#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::int128_t;
namespace multip = boost::multiprecision;

template<class F>
Int bisect(Int true_x, Int false_x, F pred) {
  while (multip::abs(true_x - false_x) > 1) {
    Int x = (true_x + false_x) / 2;
    if (pred(x)) {
      true_x = std::move(x);
    } else {
      false_x = std::move(x);
    }
  }
  return true_x;
}

constexpr i64 kBig = std::numeric_limits<i64>::max();

auto solve() -> Int {
  const i64 n = in, K = in;
  vector<i64> a(n);
  cin >> a;
  sort(ALL(a), greater{});

  auto f = [&](Int x) -> pair<Int, Int> {
    Int cnt = 0, sum = 0;
    REP(i, n) {
      if (a[i] < x) break;
      cnt += a[i] - x + 1;
      sum += (a[i] - x + 1) * (a[i] + x) / 2;
    }
    return {cnt, sum};
  };

  Int mx = bisect(kBig, 0, [&](Int x) { return f(x).first <= K; });
  const auto&[mc, ms] = f(mx);
  return ms + (K - mc) * (mx - 1);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
