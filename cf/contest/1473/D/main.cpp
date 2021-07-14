#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

using namespace std;

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

 public:
  inline int n() const { return n_; }
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

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const { return data_[1]; }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  template <bool (*pred)(const T &)>
  int max_right(int l) {
    return max_right(l, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int max_right(int l, Predicate pred) {
    assert(0 <= l && l <= n_);
    assert(pred(Monoid::id()));
    if (l == n_) return n_;
    l += offset_;
    T sm = Monoid::id();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!pred(Monoid::op(sm, data_[l]))) {
        while (l < offset_) {
          l = (2 * l);
          if (pred(Monoid::op(sm, data_[l]))) {
            sm = Monoid::op(sm, data_[l]);
            l++;
          }
        }
        return l - offset_;
      }
      sm = Monoid::op(sm, data_[l]);
      l++;
    } while ((l & -l) != l);
    return n_;
  }

  template <bool (*pred)(const T &)>
  int min_left(int r) {
    return min_left(r, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int min_left(int r, Predicate pred) {
    assert(0 <= r && r <= n_);
    assert(pred(Monoid::id()));
    if (r == 0) return 0;
    r += offset_;
    T sm = Monoid::id();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!pred(Monoid::op(data_[r], sm))) {
        while (r < offset_) {
          r = (2 * r + 1);
          if (pred(Monoid::op(data_[r], sm))) {
            sm = Monoid::op(data_[r], sm);
            r--;
          }
        }
        return r + 1 - offset_;
      }
      sm = Monoid::op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const SegTree &st) {
    os << "[";
    for (int i = 0; i < st.n(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

const i64 INF = 1e17;

struct Min {
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct Max {
  using T = long long;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }
};

void solve() {
  int n, m;
  cin >> n >> m;
  string prog;
  cin >> prog;
  assert(ssize(prog) == n);

  vector<i64> lsum(n + 1);
  {
    i64 s = 0;
    REP(i, n) {
      if (prog[i] == '+') {
        ++s;
      } else {
        --s;
      }
      lsum[i + 1] = s;
    }
  }
  SegTree<Max> segmax(lsum);
  SegTree<Min> segmin(lsum);

  REP(qi, m) {
    int l, r;
    cin >> l >> r;
    --l;
    i64 lmin = segmin.fold(0, l);
    i64 lmax = segmax.fold(0, l);
    i64 rmin = segmin.fold(r, n + 1);
    i64 rmax = segmax.fold(r, n + 1);
    i64 mn = min({0LL, lmin, lsum[l] + rmin - lsum[r]});
    i64 mx = max({0LL, lmax, lsum[l] + rmax - lsum[r]});
    i64 ans = mx - (mn - 1);
    cout << ans << "\n";
    DEBUG(l, r, lsum[l], lsum[r - 1]);
    DEBUG(lmin, lmax, rmin, rmax);
    DEBUG(mx, mn, ans);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) solve();
}
