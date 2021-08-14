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
class ClosedIntervalSet : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c] and [c+1, r].
  bool merge_adjacent;

 public:
  explicit ClosedIntervalSet(bool merge_adjacent)
      : merge_adjacent(merge_adjacent) {}

  // Returns the interval [l, r] which contains p if available.
  // Otherwise returns this->end().
  std::map<i64, i64>::iterator get(i64 p) {
    auto it = upper_bound(p);
    if (it == begin() || (--it)->second < p) return end();
    return it;
  }

  // Inserts interval [l, r]
  void insert(i64 l, i64 r) {
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
  void remove(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = upper_bound(r);
    if (itl != begin()) {
      if ((--itl)->second < l) ++itl;
    }
    if (itl == itr) return;
    i64 tl = std::min(l, itl->first);
    i64 tr = std::max(r, std::prev(itr)->second);
    erase(itl, itr);
    if (tl < l) (*this)[tl] = l - 1;
    if (r < tr) (*this)[r + 1] = tr;
  }

  // Are p and q in the same interval?
  bool same(i64 p, i64 q) {
    const auto &it = get(p);
    return it != end() && it->first <= q && q <= it->second;
  }

  i64 mex() {
    auto it = get(0);
    if (it == end()) return 0;
    return it->second + 1;
  }
};

auto solve() {
  INPUT(int, n);
  vector<pair<i64, i64>> intervals(n);
  REP(i, n) {
    INPUT(i64, l, r);
    intervals[i] = {r, l};
  }
  sort(ALL(intervals));
  ClosedIntervalSet is(true);
  for (auto[r, l] : intervals) {
    auto it = is.get(l);
    if (it == is.end()) {
      is.insert(l, l);
    } else {
      int rr = it->second;
      if (rr + 1 > r) return false;
      is.insert(rr + 1, rr + 1);
    }
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  cin >> t;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "Yes" : "No");
  }
}
