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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    return (std::upper_bound(values.begin(), values.end(), x) - values.begin()) - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

template<typename Monoid>
struct DualSegmentTree {
  using T = typename Monoid::T;

  explicit DualSegmentTree(int n) : size_(n), data_(2 * n, Monoid::id()) {}

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

  friend std::ostream &operator<<(std::ostream &os, const DualSegmentTree &st) {
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

struct SumOp {
  using T = Int;
  static T op(const T &x, const T &y) {
    return x + y;
    // alt: saturating_add(x, y)
  }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return -x; }
};
struct Event {
  Int x;
  int typ;  // 0:in, 1:query, 2:out
  Int y_top;
  Int y_bot;
  int index;
};
bool operator<(const Event &e1, const Event &e2) {
  return tie(e1.x, e1.typ) < tie(e2.x, e2.typ);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n = in, Q = in;
  vector<array<int, 4>> sites(n);
  vector<Int> ys;
  Int kSmall = Int(-5e9);
  Int kBig = Int(5e9);
  ys.push_back(kSmall);
  ys.push_back(kBig);
  vector<Event> events;
  REP(i, n) {
    array<int, 4> a;
    REP(i, 4) cin >> a[i];
    sites[i] = a;
    events.push_back({a[0] + a[2] + 1, 0, a[1] + a[2], a[1], i});
    events.push_back({a[0], 1, a[1] + a[2], a[1], i});
    ys.push_back(a[1]);
    ys.push_back(a[1] + a[2] + 1);
  }
  REP(i, Q) {
    int x = in, y = in;
    events.push_back({x, 2, y, y, i});
    ys.push_back(y);
  }
  sort(ALL(events));
  Compressed<Int> cy(ys);
  DualSegmentTree<SumOp> seg(cy.size());

  vector<Int> ans(Q, 0);
  for (const auto &e: events) {
    if (e.typ == 2) {
      int yi = cy.lindex(e.y_top);
      ans[e.index] = seg[yi];
    } else {
      const auto&[x, y, d, c] = sites[e.index];
      int yl = cy.index(e.y_bot);
      int yr = cy.index(e.y_top + 1);
      if (e.typ == 1) {
        seg.apply(yl, yr, c);
      } else {
        seg.apply(yl, yr, -c);
      }
    }
  }
  REP(i, Q) { print(ans[i]); }

}
