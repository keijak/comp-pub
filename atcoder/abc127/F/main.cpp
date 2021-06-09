#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

template <typename T>
struct Compress {
  std::vector<T> vec;

  explicit Compress(std::vector<T> v) : vec(v) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  int size() const { return vec.size(); }

  int index(T x) const {
    return (int)(lower_bound(vec.begin(), vec.end(), x) - vec.begin());
  }

  const T &value(int i) const { return vec[i]; }
};

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegTree(const std::vector<T> &leaves) : n_(leaves.size()) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
    for (int i = 0; i < n_; ++i) {
      data_[offset_ + i] = leaves[i];
    }
    for (int i = offset_ - 1; i > 0; --i) {
      data_[i] = Monoid::op(data_[i * 2], data_[i * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void set(int i, const T &x) {
    int k = offset_ + i;
    data_[k] = x;
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,k) range (0-indexed, half-open interval).
  T fold(int l, int k) const {
    l = std::max(l, 0) + offset_;
    k = std::min(k, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < k; l >>= 1, k >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (k & 1) vright = Monoid::op(data_[--k], vright);
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const { return data_[1]; }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  friend std::ostream &operator<<(std::ostream &os, const SegTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }

  template <class M, class F>
  friend int max_right(const SegTree<M> &seg, int l, F pred);

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_
};

template <class M, class F>
int max_right(const SegTree<M> &seg, int l, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= l && l <= seg.size());
  assert(pred(M::id()));
  if (l == seg.size()) return seg.size();
  l += seg.offset_;
  auto sm = M::id();
  do {
    while (l % 2 == 0) l >>= 1;
    if (!pred(M::op(sm, seg.data_[l]))) {
      while (l < seg.offset_) {
        l <<= 1;
        if (pred(M::op(sm, seg.data_[l]))) {
          sm = M::op(sm, seg.data_[l]);
          ++l;
        }
      }
      return l - seg.offset_;
    }
    sm = M::op(sm, seg.data_[l]);
    ++l;
  } while ((l & -l) != l);
  return seg.size();
}

struct Sum {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

using namespace std;

struct Query {
  bool update;
  i64 a, b;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int Q;
  cin >> Q;
  V<Query> queries(Q);
  V<i64> coords;
  coords.reserve(Q);
  REP(i, Q) {
    int qt;
    cin >> qt;
    if (qt == 1) {
      i64 a, b;
      cin >> a >> b;
      queries[i] = {true, a, b};
      coords.push_back(a);
    } else {
      assert(qt == 2);
      queries[i] = {false, 0, 0};
    }
  }

  Compress<i64> compress(move(coords));
  const int n = compress.size();
  SegTree<Sum> count_seg(n), sum_seg(n);
  i64 asum = 0, bsum = 0, total_count = 0;

  REP(i, Q) {
    if (queries[i].update) {
      ++total_count;
      asum += queries[i].a;
      bsum += queries[i].b;
      int k = compress.index(queries[i].a);
      count_seg.set(k, count_seg[k] + 1);
      sum_seg.set(k, sum_seg[k] + queries[i].a);
    } else {
      int k = max_right(count_seg, 0,
                        [&](const i64 &c) { return 2 * c < total_count; });
      i64 left_sum = sum_seg.fold(0, k);
      i64 right_sum = asum - left_sum;
      i64 left_count = count_seg.fold(0, k);
      i64 right_count = total_count - left_count;
      i64 x = compress.value(k);
      i64 minima = x * (left_count - right_count) - left_sum + right_sum + bsum;
      cout << x << ' ' << minima << '\n';
    }
  }
}
