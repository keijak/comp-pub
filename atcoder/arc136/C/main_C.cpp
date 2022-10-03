// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;
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
struct MaxOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return 0; }
};

// For each i, what's the maximum consecutive range in which a[i] is the
// minimum?
// Tie-breaking with index: comparing (a[i],i) with (a[j],j).
// Time complexity: O(n)
template<typename T>
std::vector<std::pair<int, int>> min_range(const std::vector<T> &a) {
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  // left bound
  for (int i = 0; i < n; ++i) {
    int j = i - 1;
    while (j >= 0 and a[j] >= a[i]) j = res[j].first - 1;
    res[i].first = j + 1;
  }
  // right bound
  for (int i = n - 1; i >= 0; --i) {
    int j = i + 1;
    while (j < n and a[j] >= a[i]) j = res[j].second;
    res[i].second = j;
  }
  return res;
}

auto solve() {
  const int n = in;
  const vector<int> A = in(n);
  vector<int> idx(A.size());
  std::iota(idx.begin(), idx.end(), 0);
  sort(ALL(idx), [&](int i, int j) {
    return A[i] > A[j];
  });
  DUMP(idx);

  vector<int> A3(n * 3);
  REP(i, n * 3) {
    A3[i] = A[i % n];
  }
  auto ranges = min_range(A3);
  vector<char> doneL(n * 3), doneR(n * 3);

  DualSegmentTree<MaxOp> seg(n);
  Int ans = 0;

  for (int i: idx) {
    int ai = A[i];
    int cur_fill = min(seg[i], ai);
    DUMP(seg.to_vec(), i, ai, cur_fill);
    if (cur_fill >= ai) continue;
    ans += ai - cur_fill;

    // right propagation
    int p = n + i;
    while (p < n * 3 and not doneR[p]) {
      doneR[p] = true;
      const int h = A[p % n];
      auto [_, R] = ranges[p];
      if (p >= n * 2) {
        seg.apply(p - n * 2, R - n * 2, h);
      } else if (R <= n * 2) {
        seg.apply(p - n, R - n, h);
      } else {
        seg.apply(p - n, n, h);
        seg.apply(0, R - n * 2, h);
      }
      p = R;
    }

    // left propagation
    p = n + i;
    while (p >= 0 and not doneL[p]) {
      doneL[p] = true;
      const int h = A[p % n];
      auto [L, _] = ranges[p];
      if (p < n) {
        seg.apply(L, p + 1, h);
      } else if (L >= n) {
        seg.apply(L - n, p + 1 - n, h);
      } else {
        seg.apply(L, n, h);
        seg.apply(0, p + 1 - n, h);
      }
      p = L - 1;
    }
  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
