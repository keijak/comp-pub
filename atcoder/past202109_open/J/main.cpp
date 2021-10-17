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

struct VersatileInput {
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

// Disjoint half-open intervals [l, r) (mapping l to r).
class IntervalMap : public std::map<i64, i64> {
 private:
  // If true, automatically merges [l, c) and [c, r).
  bool merge_adjacent;

 public:
  IntervalMap(bool merge_adjacent = true) : merge_adjacent(merge_adjacent) {}

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
      erase(itl, itr);
    }
    (*this)[l] = r;
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

// range-apply point-get.
template<typename Monoid>
struct DualSegTree {
  using T = typename Monoid::T;

  explicit DualSegTree(int n) : size_(n), data_(2 * n, Monoid::id()) {}

  inline int size() const { return size_; }

  // Returns i-th value.
  T operator[](int i) const {
    int k = size_ + i;
    T res = data_[k];
    while (k > 1) {
      k >>= 1;
      res = Monoid::op(res, data_[k]);
    }
    return res;
  }

  // Applies a value to [l,r) range.
  void apply(int l, int r, T val) {
    l += size_;
    r += size_;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        data_[l] = Monoid::op(data_[l], val);
        ++l;
      }
      if (r & 1) {
        --r;
        data_[r] = Monoid::op(data_[r], val);
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const DualSegTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      os << st[i];
    }
    return os << "]";
  }

 private:
  int size_;
  std::vector<T> data_;
};
struct XorOp {
  using T = unsigned long long;
  static T op(const T &x, const T &y) { return x ^ y; }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return x; }
};

auto solve() {
  int n = in, Q = in;

  DualSegTree<XorOp> seg(n * 2 + 5);
  REP(qi, Q) {
    int t = in, k = in;
    if (t == 1) {
      if (seg[k] == 1) {
        if (k > n) {
          print(n + 1 - (k - n));
        } else {
          print(n + (n + 1 - k));
        }
      } else {
        print(k);
      }
    } else {
      i64 l = n - k + 1;
      i64 r = n + k + 1;
      seg.apply(l, r, 1);
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    solve();
  }
}
