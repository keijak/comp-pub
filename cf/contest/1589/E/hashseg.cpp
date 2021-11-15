#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>

template<class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger < true>, true>>;

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
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
#define cerr if(false)cerr
#endif

using namespace std;

Int kOffset;

// range-apply point-get.
template<typename Monoid>
struct SparseDualSegmentTree {
  using T = typename Monoid::T;
  using Int = long long;

  Int size_;
  gp_hash_table<Int, T> data_;

  explicit SparseDualSegmentTree(Int n) : size_(n) {}

  inline Int size() const { return size_; }

  // Returns i-th value.
  T operator[](Int i) const {
    Int k = size_ + i;
    auto it = data_.find(k);
    T res = (it != data_.end()) ? it->second : Monoid::id();
    while (k > 1) {
      k >>= 1;
      it = data_.find(k);
      if (it != data_.end()) {
        res = Monoid::op(res, it->second);
      }
    }
    return res;
  }

  // Applies a value to [l,r) range.
  void apply(Int l, Int r, T val) {
    l += size_;
    r += size_;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        if (auto it = data_.find(l); it != data_.end()) {
          data_[l] = Monoid::op(it->second, val);
        } else {
          data_[l] = val;
        }
        ++l;
      }
      if (r & 1) {
        --r;
        if (auto it = data_.find(r); it != data_.end()) {
          data_[r] = Monoid::op(it->second, val);
        } else {
          data_[r] = val;
        }
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const SparseDualSegmentTree &st) {
    static const int kMaxOutput = 100;
    os << "[";
    for (Int i = kOffset - 10; i < kOffset + 10; ++i) {
      if (i != kOffset - 10) os << ", ";
      if (i == kOffset) {
        os << "{{" << st[i] << "}}";
      } else {
        os << st[i];
      }
    }
    if (st.size() > kMaxOutput) {
      os << ", ...";
    }
    return os << "]";
  }
};

// Keeps the value with the latest time.
struct AssignOp {
  // Value with a timestamp.
  struct T {
    int time;
    long long value;
  };
  static T op(const T &x, const T &y) { return (x.time > y.time) ? x : y; }
  static T id() { return {0, 0}; }
  friend ostream &operator<<(ostream &os, const T &val) {
    return os << "{" << val.time << ": " << val.value << "}";
  }
};

const Int kMax = 8e14;

auto solve() {
  kOffset = 4e14;
  int n = in;
  vector<Int> a = in(n);
  SparseDualSegmentTree<AssignOp> seg(kMax);
  Int ans = 0;
  REP(i, n) {
    Int x = a[i];
    if (i % 2 == 0) {
      kOffset -= x;
      seg.apply(0, kOffset, {2 * (i + 1), 0});
    } else {
      kOffset += x;
      seg.apply(kOffset + 1, kMax, {2 * (i + 1), 0});
    }
    Int delta = (i % 2 == 0) ? 1 : -1;
    Int pval = seg[kOffset + delta * x].value;
    seg.apply(kOffset + delta * x, kOffset + delta * x + 1, {2 * (i + 1) + 1, pval + 1});
    ans += seg[kOffset].value;
    //DUMP(i, ans, seg);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
