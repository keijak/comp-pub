#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
#include <algorithm>
#include <array>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

struct SuccinctBitVector {
  int size_;
  int blocks_;
  std::vector<unsigned> bit_, sum_;

  SuccinctBitVector() : size_(0), blocks_(0){};

  explicit SuccinctBitVector(int size)
      : size_(size),
        blocks_((size_ + 31) >> 5),
        bit_(blocks_, 0U),
        sum_(blocks_, 0U) {}

  void set(int k) { bit_[k >> 5] |= 1U << (k & 31); }

  void build() {
    sum_[0] = 0U;
    for (int i = 1; i < blocks_; ++i) {
      sum_[i] = sum_[i - 1] + __builtin_popcount(bit_[i - 1]);
    }
  }

  // O(1)
  bool operator[](int k) const { return (bit_[k >> 5] >> (k & 31)) & 1; }

  // O(1)
  int rank(int k) const {
    return (sum_[k >> 5] +
            __builtin_popcount(bit_[k >> 5] & ((1U << (k & 31)) - 1)));
  }

  // O(1)
  int rank(bool val, int k) const { return (val ? rank(k) : k - rank(k)); }

  // O(log(n))
  int select(bool val, int k) const {
    if (k < 0 || rank(val, size_) <= k) return (-1);
    int low = 0, high = size_;
    while (high - low > 1) {
      int mid = (low + high) >> 1;
      if (rank(val, mid) >= k + 1)
        high = mid;
      else
        low = mid;
    }
    return (high - 1);
  }

  // O(log(n))
  int select(bool val, int i, int l) const {
    return select(val, i + rank(val, l));
  }
};

template <class T, int MAXLOG = std::numeric_limits<T>::digits>
struct WaveletMatrix {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");
  int size_;
  std::array<SuccinctBitVector, MAXLOG> matrix;
  std::array<int, MAXLOG> mid;

  WaveletMatrix() = default;

  explicit WaveletMatrix(std::vector<T> v) : size_(v.size()) {
    std::vector<T> l(size_), r(size_);
    for (int level = MAXLOG - 1; level >= 0; --level) {
      matrix[level] = SuccinctBitVector(size_ + 1);
      int left = 0, right = 0;
      for (int i = 0; i < size_; ++i) {
        if (((v[i] >> level) & 1)) {
          matrix[level].set(i);
          r[right++] = v[i];
        } else {
          l[left++] = v[i];
        }
      }
      mid[level] = left;
      matrix[level].build();
      v.swap(l);
      for (int i = 0; i < right; ++i) {
        v[left + i] = r[i];
      }
    }
  }

  inline int size() const { return size_; }

  // access(i): the value at i (0-indexed).
  T operator[](int i) const {
    T ret = 0;
    for (int level = MAXLOG - 1; level >= 0; --level) {
      bool f = matrix[level][i];
      if (f) ret |= T(1) << level;
      i = matrix[level].rank(f, i) + mid[level] * f;
    }
    return ret;
  }

  // count i s.t. (0 <= i < r) && v[i] == x
  int rank(const T &x, int r) const {
    int l = 0;
    for (int level = MAXLOG - 1; level >= 0; --level) {
      std::tie(l, r) = succ((x >> level) & 1, l, r, level);
    }
    return r - l;
  }

  // k-th(0-indexed) smallest number in v[l,r)
  T kth_smallest(int l, int r, int k) const {
    assert(0 <= k && k < r - l);
    T ret = 0;
    for (int level = MAXLOG - 1; level >= 0; --level) {
      int cnt = matrix[level].rank(false, r) - matrix[level].rank(false, l);
      bool f = cnt <= k;
      if (f) {
        ret |= T(1) << level;
        k -= cnt;
      }
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // k-th(0-indexed) largest number in v[l,r)
  T kth_largest(int l, int r, int k) const {
    return kth_smallest(l, r, r - l - k - 1);
  }

  // count i s.t. (l <= i < r) && (v[i] < upper)
  int range_freq(int l, int r, T upper) const {
    int ret = 0;
    for (int level = MAXLOG - 1; level >= 0; --level) {
      bool f = ((upper >> level) & 1);
      if (f) ret += matrix[level].rank(false, r) - matrix[level].rank(false, l);
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // count i s.t. (l <= i < r) && (lower <= v[i] < upper)
  int range_freq(int l, int r, T lower, T upper) const {
    return range_freq(l, r, upper) - range_freq(l, r, lower);
  }

  // max v[i] s.t. (l <= i < r) && (v[i] < upper)
  T prev_value(int l, int r, T upper) const {
    int cnt = range_freq(l, r, upper);
    return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
  }

  // min v[i] s.t. (l <= i < r) && (lower <= v[i])
  T next_value(int l, int r, T lower) const {
    int cnt = range_freq(l, r, lower);
    return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
  }

 private:
  std::pair<int, int> succ(bool f, int l, int r, int level) const {
    return {matrix[level].rank(f, l) + mid[level] * f,
            matrix[level].rank(f, r) + mid[level] * f};
  }
};

// T: can be large nubmers or negative numbers
// Number of unique values must be less than 2^MAXLOG.
template <typename T, int MAXLOG = 28>
struct CompressedWaveletMatrix {
  std::vector<T> values;
  WaveletMatrix<size_t, MAXLOG> indices;

  explicit CompressedWaveletMatrix(const std::vector<T> &v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    assert(values.size() < (1ULL << MAXLOG));

    std::vector<size_t> t(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
      t[i] = index(v[i]);
    }
    indices = WaveletMatrix<size_t, MAXLOG>(t);
  }

  T operator[](int k) const { return values[indices[k]]; }

  int rank(const T &x, int r) const {
    auto pos = index(x);
    if (pos == values.size() || values[pos] != x) return 0;
    return indices.rank(pos, r);
  }

  T kth_smallest(int l, int r, int k) const {
    return values[indices.kth_smallest(l, r, k)];
  }

  T kth_largest(int l, int r, int k) const {
    return values[indices.kth_largest(l, r, k)];
  }

  int range_freq(int l, int r, T upper) const {
    return indices.range_freq(l, r, index(upper));
  }

  int range_freq(int l, int r, T lower, T upper) const {
    return indices.range_freq(l, r, index(lower), index(upper));
  }

  T prev_value(int l, int r, T upper) const {
    auto ret = indices.prev_value(l, r, index(upper));
    return ret == -1 ? T(-1) : values[ret];
  }

  T next_value(int l, int r, T lower) const {
    auto ret = indices.next_value(l, r, index(lower));
    return ret == -1 ? T(-1) : values[ret];
  }

 private:
  // value -> index
  size_t index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};

// tour: preorder node ids
// The interval [begin[v], end[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<unsigned> tour;  // (depth)
  vector<int> begin;      // index in the tour on entering the subtree of v
  vector<int> end;        // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()), g(move(g)), begin(n, -1), end(n, -1), depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    begin[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.emplace_back(depth[v]);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
    }
    end[v] = int(tour.size());
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  vector<vector<int>> g(n);
  for (int i = 1; i < n; ++i) {
    int p;
    cin >> p;
    g[p - 1].push_back(i);
  }
  EulerTour et(g, 0);
  WaveletMatrix<unsigned, 18> wm(et.tour);

  int Q;
  cin >> Q;
  REP(q, Q) {
    unsigned u, d;
    cin >> u >> d;
    --u;
    unsigned ud = et.depth[u];
    if (d < ud) {
      cout << "0\n";
    } else if (d == ud) {
      cout << "1\n";
    } else {
      int count = wm.rank(d, et.end[u]) - wm.rank(d, et.begin[u]);
      cout << count << '\n';
    }
  }
}
