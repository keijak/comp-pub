#include <bits/stdc++.h>
#include <atcoder/string>

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

template<typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Log base 2 of the most significant bit that is set to 1.
inline int msb_log(unsigned x) {
  assert(x != 0);
  return num_bits<unsigned> - __builtin_clz(x) - 1;
}

// Range Min/Max Query based on Fischer-Heun Structure.
// - Initialization: O(n)
// - Query: O(1)
//
// Usage:
//   RMQ rmq(a.size(), [&](int i, int j){ return a[i] < a[j]; });
//   auto minval = a[rmq.fold(l, r)];
template<class BetterOp, class mask_t = unsigned>
struct RMQ {
  static_assert(std::is_integral_v<mask_t>, "mask_t must be integral");
  static_assert(std::is_unsigned_v<mask_t>, "mask_t must be unsigned");
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);
  static constexpr int block_size_ = num_bits<mask_t>;

  int n_;                 // sequence size
  int block_count_;       // total number of blocks
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.
  std::vector<mask_t> indicator_;
  std::vector<std::vector<int>> sparse_table_;

  RMQ(int n, BetterOp better)
      : n_(n),
        block_count_((n_ + block_size_ - 1) / block_size_),
        better_than_(std::move(better)),
        indicator_(n_),
        sparse_table_(
            block_count_ == 0 ? 0 : msb_log(unsigned(block_count_)) + 1,
            std::vector<int>(block_count_)) {
    static constexpr int bufsize = block_size_ + 1;
    static int buf[bufsize];       // ring buffer [lp,rp)
    int lp = 1, rp = 1, rpm1 = 0;  // rpm1 = rp-1 (mod bufsize)

    // Build the indicator table.
    for (int r = 0; r < n_; ++r) {
      while (lp != rp and r - buf[lp] >= block_size_) {
        ++lp;
        if (lp == bufsize) lp = 0;
      }
      while (lp != rp and not better_than_(buf[rpm1], r)) {
        rp = rpm1--;
        if (rp == 0) rpm1 = bufsize - 1;
      }
      indicator_[r] = 1;
      if (lp != rp) {
        const int p = buf[rpm1];
        indicator_[r] |= (indicator_[p] << (r - p));
      }
      buf[rp] = r;
      rpm1 = rp++;
      if (rp == bufsize) rp = 0;
    }

    // Build the sparse table.
    for (int i = 0; i < block_count_; ++i) {
      sparse_table_[0][i] =
          best_index_small(std::min(block_size_ * (i + 1), n_) - 1);
    }
    for (int i = 0, height = int(sparse_table_.size()) - 1; i < height; ++i) {
      for (int j = 0; j < block_count_; ++j) {
        sparse_table_[i + 1][j] = better_index(
            sparse_table_[i][j],
            sparse_table_[i][std::min(j + (1 << i), block_count_ - 1)]);
      }
    }
  }

  // Returns the index of the best value in [l, r) (half-open interval).
  inline int fold(int l, int r) const {
    assert(l < r);
    // Internally use closed interval.
    return best_index(l, r - 1);
  }

 private:
  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }

  // Returns the index of the best value in [r - width, r] (closed interval).
  inline int best_index_small(int r, int width = block_size_) const {
    assert(r < n_);
    assert(width > 0);
    assert(width <= block_size_);
    mask_t ind = indicator_[r];
    if (width < block_size_) {
      ind &= (mask_t(1) << width) - 1;
    }
    return r - msb_log(ind);
  }

  // Returns the index of the best value in [l, r] (closed interval).
  inline int best_index(int l, int r) const {
    l = std::clamp(l, 0, n_ - 1);
    r = std::clamp(r, 0, n_ - 1);
    const int width = r - l + 1;
    if (width <= block_size_) {
      return best_index_small(r, width);
    }
    const int al = best_index_small(std::min(l + block_size_, n_) - 1);
    const int ar = best_index_small(r);
    int ans = better_index(al, ar);

    const int bl = l / block_size_ + 1;
    const int br = r / block_size_ - 1;
    if (bl <= br) {
      const int k = msb_log(unsigned(br - bl + 1));
      const int bl2 = br - (1 << k) + 1;
      const int am = better_index(sparse_table_[k][bl], sparse_table_[k][bl2]);
      ans = better_index(ans, am);
    }
    return ans;
  }
};

// Range Min/Max Query based on Sparse Table.
// - Initialization: O(n*log(n))
// - Query: O(1)
//
// Usage:
//   SparseTableRMQ rmq(a.size(), [&](int i, int j){ return a[i] < a[j]; });
//   auto minval = a[rmq.fold(l, r)];
template<class BetterOp>
struct SparseTableRMQ {
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);

  int n_;                 // sequence size
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.
  std::vector<std::vector<int>> sparse_table_;

  SparseTableRMQ(int n, BetterOp better)
      : n_(n),
        better_than_(std::move(better)),
        sparse_table_(n == 0 ? 1 : (msb_log(n) + 1), std::vector<int>(n)) {
    for (int j = 0; j < n; ++j) {
      sparse_table_[0][j] = j;
    }
    for (int i = 0, height = int(sparse_table_.size()) - 1; i < height; ++i) {
      for (int j = 0; j < n; ++j) {
        sparse_table_[i + 1][j] =
            better_index(sparse_table_[i][j],
                         sparse_table_[i][std::min(j + (1 << i), n - 1)]);
      }
    }
  }

  // Returns the index of the best value in [l, r) (half-open interval).
  inline int fold(int l, int r) const {
    assert(0 <= l and l < r);
    assert(1 <= r and r <= n_);
    const int k = msb_log(r - l);
    const int l2 = r - (1 << k);
    return better_index(sparse_table_[k][l], sparse_table_[k][l2]);
  }

 private:
  // Log base 2 of the most significant bit that is set to 1.
  static inline int msb_log(unsigned x) {
    return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
  }

  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }
};

auto solve() {
  INPUT(int, n);
  INPUT(string, s);
  const std::vector<int> sa = atcoder::suffix_array(s);
  const std::vector<int> lcp = atcoder::lcp_array(s, sa);
  DUMP(sa);
  DUMP(lcp);
  SparseTableRMQ rmq(lcp.size(), [&](int i, int j) { return lcp[i] < lcp[j]; });
  std::vector<int> rsa(n);  // the position of s[i:] in the suffix array.
  for (int i = 0; i < n; ++i) rsa[sa[i]] = i;

  REP(k, n) {
    i64 res = 0;
    REP(i, n) {
      int p = std::min(rsa[i], rsa[k]);
      int q = std::max(rsa[i], rsa[k]);
      if (p == q) {
        res += n - k;
      } else {
        int j = rmq.fold(p, q);
        res += lcp[j];
      }
    }
    print(res);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
