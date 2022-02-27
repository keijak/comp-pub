#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) {
    return x + y;
  }
  static constexpr T id() { return 0; }
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

  std::vector<T> to_vec(int sz = -1) const {
    if (sz < 0 or sz > size()) sz = size();
    std::vector<T> res(sz);
    for (int i = 0; i < sz; ++i) res[i] = (*this)[i];
    return res;
  }

 private:
  int size_;
  std::vector<T> data_;
};

struct Interval {
  Int l, r;
  int color;
};
ostream &operator<<(ostream &os, const Interval &iv) {
  return os << "{[" << iv.l << ", " << iv.r << ") => " << iv.color << "}";
}

struct EventHandler {
  vector<Int> lazy;
  DualSegmentTree<SumOp> seg;

  explicit EventHandler(int n) : lazy(n, 0LL), seg(n) {}

  void on_add(const Interval &iv) {
    seg.apply(iv.l, iv.r, -lazy[iv.color]);
  }

  void on_remove(const Interval &iv) {
    seg.apply(iv.l, iv.r, lazy[iv.color]);
  }

  Int query(int x, int c) {
    return seg[x] + lazy[c];
  }
};

class IntervalMap : public std::map<Int, Interval> {
 public:
  EventHandler event_handler_;

  explicit IntervalMap(EventHandler eh) : event_handler_(move(eh)) {}

  // Returns the interval which contains p if available.
  // Otherwise returns this->end().
  std::map<Int, Interval>::iterator find_interval(Int p) {
    auto it = upper_bound(p);
    if (it != begin()) {
      --it;
      if (it->second.r > p) return it;
    }
    return end();
  }

  // Sets interval [l, r) => value.
  void set_interval(const Interval &interval) {
    if (interval.l >= interval.r) return;  // empty interval
    remove_interval(interval.l, interval.r);
    event_handler_.on_add(interval);
    (*this)[interval.l] = interval;
  }

  // Removes interval [l, r)
  void remove_interval(Int l, Int r) {
    if (l >= r) return;  // empty interval
    auto itl = upper_bound(l), itr = lower_bound(r);
    if (itl != begin()) {
      --itl;
      if (itl->second.r <= l) ++itl;
    }
    if (itl == itr) return;
    std::optional<Interval> ltip, rtip;
    if (itl->first < l) {
      ltip = itl->second;
      assert(ltip->r >= l);
      ltip->r = l;
    }
    if (auto riv = std::prev(itr)->second; riv.r > r) {
      rtip = riv;
      assert(rtip->l <= r);
      rtip->l = r;
    }
    for (auto it = itl; it != itr; it = erase(it)) {
      event_handler_.on_remove(it->second);
    }
    if (ltip) {
      event_handler_.on_add(*ltip);
      (*this)[ltip->l] = *ltip;
    }
    if (rtip) {
      event_handler_.on_add(*rtip);
      (*this)[rtip->l] = *rtip;
    }
  }
};

auto solve() {
  int n = in, Q = in;
  EventHandler eh(n + 1);
  IntervalMap im(eh);
  im.set_interval(Interval{0, n, 1});
  REP(qi, Q) {
    string qtype = in;
    if (qtype == "Color") {
      int l = in, r = in, c = in;
      --l;
      im.set_interval({l, r, c});
    } else if (qtype == "Add") {
      int c = in;
      Int x = in;
      im.event_handler_.lazy[c] += x;
    } else if (qtype == "Query") {
      int i = in;
      --i;
      auto it = im.find_interval(i);
      assert(it != im.end());
      int color = it->second.color;
      print(im.event_handler_.query(i, color));
    } else {
      DUMP(qtype);
      assert(false);
    }
  }
  return Void();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
