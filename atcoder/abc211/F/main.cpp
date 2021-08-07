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

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return -x; }
};

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

struct Event {
  int x;
  int type;  // 1: rect, 2: query
  int y1;    // y1 < y2
  int y2;
  int poly;
};
ostream &operator<<(ostream &os, const Event &e) {
  if (e.type == 1) {
    os << "{x=" << e.x << ", rect(y1=" << e.y1 << ", y2=" << e.y2 << ", poly=" << e.poly << ")}";
  } else {
    os << "{x=" << e.x << ", query(y1=" << e.y1 << ", query=" << e.poly << ")}";
  }
  return os;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  INPUT(int, n);
  vector<Event> events;
  REP(i, n) {
    INPUT(int, m);
    vector<int> xs(m), ys(m);
    REP(j, m) {
      cin >> xs[j] >> ys[j];
    }
    for (int j = 0; j < m - 1; j += 2) {
      assert(xs[j] == xs[j + 1]);
      int y1 = ys[j], y2 = ys[j + 1];
      if (y1 > y2) swap(y1, y2);
      assert(y1 != y2);
      events.push_back(Event{xs[j], 1, y1, y2, i});
    }
  }

  INPUT(int, Q);
  REP(qi, Q) {
    INPUT(int, x, y);
    events.push_back(Event{x, 2, y, y, qi});
  }
  sort(ALL(events), [&](const Event &e1, const Event &e2) {
    return tie(e1.x, e1.type, e1.poly) < tie(e2.x, e2.type, e2.poly);
  });

  const int L = 100100;
  DualSegTree<SumOp> seg(L);
  vector<ClosedIntervalSet> is(n, ClosedIntervalSet(true));
  vector<int> ans(Q, -1);

  for (const Event &e : events) {
    //DUMP(e);
    if (e.type == 1) {
      auto &s = is[e.poly];
      auto it = s.get(e.y1);
      if (it == s.end()) {
        s.insert(e.y1, e.y2 - 1);
        seg.apply(e.y1, e.y2, 1);
      } else {
        s.remove(e.y1, e.y2 - 1);
        seg.apply(e.y1, e.y2, -1);
      }
    } else {
      ans[e.poly] = seg[e.y1];
    }
  }
  print_seq(ans, "\n");
}
