#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
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

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
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

using namespace std;

template <typename LazyMonoid>
struct LazySegTree {
  using T = typename LazyMonoid::T;
  using F = typename LazyMonoid::F;

  inline int n() const { return n_; }
  inline int offset() const { return offset_; }

  explicit LazySegTree(int n)
      : LazySegTree(std::vector<T>(n, LazyMonoid::id())) {}

  explicit LazySegTree(const std::vector<T> &v) : n_(int(v.size())) {
    offset_ = 1;
    for (bits_ = 0; offset_ < n_; ++bits_) {
      offset_ <<= 1;
    }
    data_.assign(2 * offset_, LazyMonoid::id());
    lazy_ = std::vector<F>(offset_, LazyMonoid::f_id());
    for (int i = 0; i < n_; i++) data_[offset_ + i] = v[i];
    for (int i = offset_ - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < n_);
    p += offset_;
    // Update the leaf.
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = x;
    // Update its ancestors.
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }

  const T &operator[](int p) const {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    return data_[p];
  }

  T fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return LazyMonoid::id();

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push(r >> i);
    }

    T sml = LazyMonoid::id(), smr = LazyMonoid::id();
    while (l < r) {
      if (l & 1) sml = LazyMonoid::op(sml, data_[l++]);
      if (r & 1) smr = LazyMonoid::op(data_[--r], smr);
      l >>= 1;
      r >>= 1;
    }

    return LazyMonoid::op(sml, smr);
  }

  void apply(int p, F f) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = LazyMonoid::apply(f, data_[p]);
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }
  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return;

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }

    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1) all_apply(l++, f);
        if (r & 1) all_apply(--r, f);
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }

    for (int i = 1; i <= bits_; i++) {
      if (((l >> i) << i) != l) update(l >> i);
      if (((r >> i) << i) != r) update((r - 1) >> i);
    }
  }

  template <bool (*g)(T)>
  int max_right(int l) const {
    return max_right(l, [](T x) { return g(x); });
  }
  template <class G>
  int max_right(int l, G g) const {
    assert(0 <= l && l <= n_);
    assert(g(LazyMonoid::id()));
    if (l == n_) return n_;
    l += offset_;
    for (int i = bits_; i >= 1; i--) push(l >> i);
    T sm = LazyMonoid::id();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!g(LazyMonoid::op(sm, data_[l]))) {
        while (l < offset_) {
          push(l);
          l = (2 * l);
          if (g(LazyMonoid::op(sm, data_[l]))) {
            sm = LazyMonoid::op(sm, data_[l]);
            l++;
          }
        }
        return l - offset_;
      }
      sm = LazyMonoid::op(sm, data_[l]);
      l++;
    } while ((l & -l) != l);
    return n_;
  }

  template <bool (*g)(T)>
  int min_left(int r) const {
    return min_left(r, [](T x) { return g(x); });
  }
  template <class G>
  int min_left(int r, G g) const {
    assert(0 <= r && r <= n_);
    assert(g(LazyMonoid::id()));
    if (r == 0) return 0;
    r += offset_;
    for (int i = bits_; i >= 1; i--) push((r - 1) >> i);
    T sm = LazyMonoid::id();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!g(LazyMonoid::op(data_[r], sm))) {
        while (r < offset_) {
          push(r);
          r = (2 * r + 1);
          if (g(LazyMonoid::op(data_[r], sm))) {
            sm = LazyMonoid::op(data_[r], sm);
            r--;
          }
        }
        return r + 1 - offset_;
      }
      sm = LazyMonoid::op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const LazySegTree &st) {
    os << "[";
    for (int i = 0; i < st.n(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }

 private:
  void update(int k) {
    data_[k] = LazyMonoid::op(data_[2 * k], data_[2 * k + 1]);
  }
  void all_apply(int k, F f) const {
    data_[k] = LazyMonoid::f_apply(f, data_[k]);
    if (k < offset_) lazy_[k] = LazyMonoid::f_compose(f, lazy_[k]);
  }
  void push(int k) const {
    all_apply(2 * k, lazy_[k]);
    all_apply(2 * k + 1, lazy_[k]);
    lazy_[k] = LazyMonoid::f_id();
  }

  int n_, offset_, bits_;
  mutable std::vector<T> data_;
  mutable std::vector<F> lazy_;
};

using Real = long double;

struct AssignMin {
  struct T {
    Real sum;
    int size;
    Real min_sum;
    int min_size;
  };
  using F = std::optional<Real>;

  // Fold: Sum&Min
  static T op(const T &x, const T &y) {
    Real sm = x.sum + y.sum;
    int sz = x.size + y.size;
    if (x.min_size == 0 or ((x.sum + y.min_sum) / (x.size + y.min_size) <=
                            (x.min_sum / x.min_size) + 1e-8)) {
      return {sm, sz, x.sum + y.min_sum, x.size + y.min_size};
    } else {
      return {sm, sz, x.min_sum, x.min_size};
    }
  }
  static constexpr T id() { return {0.0, 0, 0.0, 0}; }

  // Update: Assign
  static T f_apply(const F &f, const T &x) {
    if (not f.has_value()) return x;
    Real sm = f.value() * x.size;
    return {sm, x.size, sm, x.size};
  }
  static F f_compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F f_id() { return std::nullopt; }
};

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  cout << fixed << setprecision(9);
  //  cerr << fixed << setprecision(9);

  int n;
  cin >> n;
  V<AssignMin::T> a(n);
  REP(i, n) {
    int x;
    cin >> x;
    a[i] = {Real(x), 1, Real(x), 1};
  }
  LazySegTree<AssignMin> seg(a);
  V<Real> ans(n);
  REP(i, n) {
    auto x = seg.fold(i, n);
    ans[i] = x.min_sum / x.min_size;
    seg.apply(i, i + x.min_size, ans[i]);
  }
  REP(i, n) { cout << ans[i] << '\n'; }
}
