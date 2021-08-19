#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
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

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Set of disjoint closed intervals [l, r].
class ClosedIntervalSet : public std::map<Int, Int> {
 private:
  // If true, automatically merges [l, c] and [c+1, r].
  bool merge_adjacent;

 public:
  ClosedIntervalSet(bool merge_adjacent = true)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r] which contains p if available.
  // Otherwise returns this->end().
  std::map<Int, Int>::iterator get(Int p) {
    auto it = upper_bound(p);
    if (it == begin() || (--it)->second < p) return end();
    return it;
  }

  // Inserts interval [l, r]
  void insert(Int l, Int r) {
    auto itl = upper_bound(l), itr = upper_bound(r + merge_adjacent);
    if (itl != begin()) {
      if ((--itl)->second < l - merge_adjacent) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      erase(itl, itr);
    }
    (*this)[l] = r;
  }

  // Removes interval [l, r]
  void remove(Int l, Int r) {
    auto itl = upper_bound(l), itr = upper_bound(r);
    if (itl != begin()) {
      if ((--itl)->second < l) ++itl;
    }
    if (itl == itr) return;
    Int tl = std::min(l, itl->first);
    Int tr = std::max(r, std::prev(itr)->second);
    erase(itl, itr);
    if (tl < l) (*this)[tl] = l - 1;
    if (r < tr) (*this)[r + 1] = tr;
  }
};

// Returns x mod y.
// Guarantees 0 <= r < y (even when x is negative).
Int floor_mod(Int x, Int y) {
  assert(y > 0);
  Int r = x % y;
  if (r < 0) r += y;
  return r;
}

auto solve() -> Int {
  INPUT(Int, n, X, D);
  if (D == 0) {
    if (X == 0) return 1;
    return n + 1;
  }
  if (D < 0) {
    X *= -1;
    D *= -1;
  }
  Int total = n * X + n * (n - 1) / 2 * D;
  DUMP(total);
  map<Int, ClosedIntervalSet> iss;
  for (Int i = 0; i <= n; ++i) {
    Int r = floor_mod(total - i * X * 2, D * 2);
    Int upper = total - (i * X * 2 + i * (i - 1) * D);
    Int lower = total - (i * X * 2 + i * (2 * n - 1 - i) * D);
    if (upper < lower) swap(lower, upper);
    iss[r].insert(lower, upper);
  }
  Int ans = 0;
  for (const auto&[rem, is] : iss) {
    for (const auto&[l, r] : is) {
      ans += (r - l) / (D * 2) + 1;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
