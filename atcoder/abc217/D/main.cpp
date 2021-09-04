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

// Disjoint half-open intervals [l, r) (mapping l to r).
class HalfOpenIntervalSet : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c) and [c, r].
  bool merge_adjacent;

 public:
  HalfOpenIntervalSet(bool merge_adjacent = true)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r) which contains p if available.
  // Otherwise returns this->end().
  std::map<i64, i64>::iterator find_interval(i64 p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second > p) return it;
    }
    return end();
  }

  // Inserts interval [l, r)
  void add_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r - 1 + merge_adjacent);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l - merge_adjacent) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      erase(itl, itr);
    }
    (*this)[l] = r;
  }

  // Removes interval [l, r)
  void remove_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r - 1);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l) ++itl;
    }
    if (itl == itr) return;
    i64 tl = std::min(l, itl->first);
    i64 tr = std::max(r, std::prev(itr)->second);
    DUMP(l, r, tl, tr);
    erase(itl, itr);
    if (tl < l) (*this)[tl] = l;
    if (r < tr) (*this)[r] = tr;
  }

  // Are p and q in the same interval?
  bool same(i64 p, i64 q) {
    const auto it = find_interval(p);
    return it != end() and it->first <= q and q < it->second;
  }

  // Minimum excluded value greater than or equal to X.
  i64 mex(int bottom = 0) {
    const auto it = find_interval(bottom);
    if (it == end()) return bottom;
    return it->second;
  }
};

auto solve() {
  HalfOpenIntervalSet cis(false);
  int L = in, Q = in;
  cis.add_interval(0, L);
  REP(qi, Q) {
    int c = in, x = in;
    DUMP(qi, cis, c, x);
    auto it = cis.find_interval(x - 1);
    assert(it != cis.end());
    assert(x < it->second);
    int l = it->first;
    int r = it->second;
    if (c == 1) {
      DUMP(qi, l, r);
      cis.remove_interval(l, r);
      DUMP(cis);
      cis.add_interval(l, x);
      DUMP(cis);
      cis.add_interval(x, r);
    } else {
      print(r - l);
    }
  }
}

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    solve();
  }
}
