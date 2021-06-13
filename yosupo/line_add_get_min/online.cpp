#include <bits/stdc++.h>
using i64 = long long;
using namespace std;

// Enables `__gnu_pbds::gp_hash_table::resize()` similar to
// `std::unordered_map::reserve()`.
#include <ext/pb_ds/assoc_container.hpp>

template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;

struct Min {
  using T = long long;
  static bool less(const T &x, const T &y) { return x < y; }
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

// Line-like function.
//
// Not necessarily a line, but must hold the transcending property:
// Two curves cross at most once in the specified range.
template <class T>
struct Line {
  T a;
  T b;

  Line(T a, T b) : a(std::move(a)), b(std::move(b)) {}

  T eval(T x) const { return a * x + b; }

  static Line constant(T value) { return Line(0, value); }
};

template <class Op>  // Min or Max
struct SparseLiChaoTree {
  using Y = typename Op::T;  // y-coordinate
  using X = i64;             // x-coordinate
  static_assert(std::is_arithmetic<Y>::value, "must be number");
  static_assert(std::is_integral<X>::value, "must be integer");

  // Creates a Li-Chao Tree with x-coordinate bounds [x_low, x_high).
  // Dynamically creates nodes.
  SparseLiChaoTree(X x_low, X x_high) : x_low_(x_low), n_(x_high - x_low) {
    assert(x_low <= x_high);
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    lines_.resize(1 << 20);
  }

  // Adds y = g(x).
  void add_line(Line<Y> g) { update(std::move(g), 0, n_); }

  // Adds y = g(x) in xl <= x < xr.
  void add_segment(Line<Y> g, X xl, X xr) {
    auto l = xl - x_low_;
    auto r = xr - x_low_;
    update(std::move(g), l, r);
  }

  // Returns the minimum/maximum f(x) at x.
  Y query(X x) const {
    assert(x < n_);
    Y y = Op::id();
    for (X i = x - x_low_ + offset_; i > 0; i >>= 1) {
      auto it = lines_.find(i);
      if (it != lines_.end()) {
        y = Op::op(y, it->second.eval(x));
      }
    }
    return y;
  }

 private:
  void update(Line<Y> g, X l, X r) {
    for (l += offset_, r += offset_; l < r; l >>= 1, r >>= 1) {
      if (l & 1) descend(g, l++);
      if (r & 1) descend(g, --r);
    }
  }

  void descend(Line<Y> g, X i) {
    X l = i, r = i + 1;
    while (l < offset_) {
      l <<= 1;
      r <<= 1;
    }
    while (l < r) {
      auto c = (l + r) >> 1;
      auto xl = l - offset_ + x_low_;
      auto xc = c - offset_ + x_low_;
      auto xr = r - 1 - offset_ + x_low_;
      auto fit = lines_.find(i);
      if (fit == lines_.end()) {
        lines_.insert({i, std::move(g)});
        return;
      }
      Line<Y> &f = fit->second;
      if (not Op::less(g.eval(xl), f.eval(xl)) and
          not Op::less(g.eval(xr), f.eval(xr))) {
        return;
      }
      if (not Op::less(f.eval(xl), g.eval(xl)) and
          not Op::less(f.eval(xr), g.eval(xr))) {
        f = std::move(g);
        return;
      }
      if (Op::less(g.eval(xc), f.eval(xc))) {
        std::swap(f, g);
      }
      if (Op::less(g.eval(xl), f.eval(xl))) {
        i = (i << 1) | 0;
        r = c;
      } else {
        i = (i << 1) | 1;
        l = c;
      }
    }
  }

  X x_low_, n_, offset_;
  gp_hash_table<X, Line<Y>> lines_;
};

struct Query {
  int qtype;
  i64 a, b;
  i64 p;
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;

  SparseLiChaoTree<Min> lct(-1'000'000'005, 1'000'000'005);

  for (int i = 0; i < N; ++i) {
    Query q;
    q.qtype = 0;
    cin >> q.a >> q.b;
    lct.add_line(Line<i64>(q.a, q.b));
  }
  for (int i = 0; i < Q; ++i) {
    Query q;
    cin >> q.qtype;
    if (q.qtype == 0) {
      cin >> q.a >> q.b;
      lct.add_line(Line<i64>(q.a, q.b));
    } else {
      cin >> q.p;
      cout << lct.query(q.p) << "\n";
    }
  }
}
