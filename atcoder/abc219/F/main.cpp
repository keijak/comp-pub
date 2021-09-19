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
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Disjoint half-open intervals [l, r) (mapping l to r).
class IntervalMap : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c) and [c, r).
  bool merge_adjacent;
  i64 length;

 public:
  IntervalMap(bool merge_adjacent = true) : merge_adjacent(merge_adjacent), length(0) {}

  i64 total_length() const {
    return this->length;
  }

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
    auto itl = upper_bound(l), itr = lower_bound(r + merge_adjacent);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l - merge_adjacent) ++itl;
    }
    if (itl != itr) {
      l = std::min(l, itl->first);
      r = std::max(r, std::prev(itr)->second);
      while (itl != itr) {
        length -= itl->second - itl->first;
        itl = erase(itl);
      }
      //erase(itl, itr);
    }
    (*this)[l] = r;
    length += r - l;
  }

  // Removes interval [l, r)
  void remove_interval(i64 l, i64 r) {
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second <= l) ++itl;
    }
    if (itl == itr) return;
    i64 tl = std::min(l, itl->first);
    i64 tr = std::max(r, std::prev(itr)->second);
    while (itl != itr) {
      length -= itl->second - itl->first;
      itl = erase(itl);
    }
    if (tl < l) {
      (*this)[tl] = l;
      length += l - tl;
    }
    if (r < tr) {
      (*this)[r] = tr;
      length += tr - r;
    }
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

template<class T>
T floor_div(T x, T y) {
  assert(y != 0);
  return x / y - (((x ^ y) < 0) and (x % y));
}

template<class T>
T floor_mod(T x, T y) {
  assert(y > 0);
  auto r = x % y;
  if (r < 0) r += y;
  return r;
}

auto solve() -> i64 {
  const string S = in;
  const i64 K = in;
  const int n = ssize(S);
  i64 gx = 0, gy = 0;
  set<pair<i64, i64>> trace;
  for (auto ch: S) {
    trace.emplace(gx, gy);
    if (ch == 'L') {
      --gx;
    } else if (ch == 'R') {
      ++gx;
    } else if (ch == 'U') {
      ++gy;
    } else {
      --gy;
    }
  }
  trace.emplace(gx, gy);
  DUMP(gx, gy);
  {
    set<pair<i64, i64>> tmp;
    for (auto[x, y]: trace) {
      if (gx < 0) x *= -1;
      if (gy < 0) y *= -1;
      if (gx == 0) swap(x, y);
      tmp.emplace(x, y);
    }
    tmp.swap(trace);

    gx = abs(gx);
    gy = abs(gy);
    if (gx == 0) swap(gx, gy);
  }

  if (gx == 0 and gy == 0) {
    set<pair<i64, i64>> ps;
    for (const auto&[x, y]: trace) {
      ps.insert({x, y});
    }
    return ssize(ps);
  }

  if (gy == 0) {
    map<pair<i64, i64>, set<pair<i64, i64>>> mp;
    for (const auto&[x, y]: trace) {
      i64 nx = floor_mod<i64>(x, gx);
      i64 ny = y;
      mp[{nx, ny}].insert({x, y});
    }
    i64 ans = 0;
    for (const auto&[np, vec]: mp) {
      IntervalMap imap;
      for (const auto&[x, y]: vec) {
        i64 qx = floor_div(x, gx);
        imap.add_interval(qx, qx + K);
      }
      ans += imap.total_length();
    }
    return ans;
  }
  {
    map<tuple<i64, i64, i64>, set<pair<i64, i64>>> mp;
    for (const auto&[x, y]: trace) {
      i64 nx = floor_mod<i64>(x, gx);
      i64 ny = floor_mod<i64>(y, gy);
      i64 na = floor_div(x, gx) - floor_div(y, gy);
      mp[{nx, ny, na}].insert({x, y});
    }
    i64 ans = 0;
    for (const auto&[np, vec]: mp) {
      IntervalMap imap;
      for (const auto&[x, y]: vec) {
        i64 qx = floor_div(x, gx);
        imap.add_interval(qx, qx + K);
      }
      ans += imap.total_length();
    }
    return ans;
  }
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
