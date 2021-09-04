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

struct Interval {
  i64 r;
  i64 val;
};
ostream &operator<<(ostream &os, const Interval &iv) {
  return os << "{r=" << iv.r << ", val=" << iv.val << "}";
}

// Disjoint half-open intervals [l, r) (mapping l to r).
class IntervalMap : public std::map<i64, Interval> {
 public:
  // If true, automatically merges [l, c) and [c, r).
  bool merge_adjacent;
  map<int, i64> val_counts;
  i64 total;

  IntervalMap(bool merge_adjacent = true) : merge_adjacent(merge_adjacent), total(0) {}

  // Returns the interval [l, r) which contains p if available.
  // Otherwise returns this->end().
  std::map<i64, Interval>::iterator find_interval(i64 p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second.r > p) return it;
    }
    return end();
  }

  // Inserts interval [l, r)
  void add_interval(i64 l, i64 r, i64 val) {
    auto itl = upper_bound(l), itr = lower_bound(r + merge_adjacent);
    if (itl != begin()) {
      --itl;
      if (itl->second.r <= l - merge_adjacent) ++itl;
    }

    if (itl != itr) {
      vector<tuple<i64, i64, i64>> restore;
      if (itl->first < l) {
        restore.push_back({itl->first, l, itl->second.val});
      }
      Interval ivr = std::prev(itr)->second;
      if (ivr.r > r) {
        restore.push_back({r, ivr.r, ivr.val});
      }
      for (; itl != itr;) {
        i64 width = itl->second.r - itl->first;

        i64 p = val_counts[itl->second.val];
        total -= p * (p - 1) / 2;
        val_counts[itl->second.val] -= width;
        p = val_counts[itl->second.val];
        total += p * (p - 1) / 2;

        itl = erase(itl);
      }

      for (const auto&[al, ar, aval]: restore) {
        (*this)[al] = {ar, aval};

        i64 p = val_counts[aval];
        total -= p * (p - 1) / 2;
        val_counts[aval] += ar - al;
        p = val_counts[aval];
        total += p * (p - 1) / 2;
      }
    }
    (*this)[l] = {r, val};

    i64 p = val_counts[val];
    total -= p * (p - 1) / 2;
    val_counts[val] += r - l;
    p = val_counts[val];
    total += p * (p - 1) / 2;
  }
};

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in;
  vector<int> a(n);
  cin >> a;
  IntervalMap im(false);
  REP(i, n) {
    im.add_interval(i, i + 1, a[i]);
  }
  DUMP(im.val_counts);
  DUMP(im.total);

  const int Q = in;
  REP(qi, Q) {
    int l = in, r = in, x = in;
    --l;
    im.add_interval(l, r, x);
    DUMP(qi, im);
    DUMP(im.val_counts);
    DUMP(im.total);
    print(im.total);
  }
}
