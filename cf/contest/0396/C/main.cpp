#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

using namespace std;

template <unsigned M>
struct ModInt {
  constexpr ModInt(long long val = 0) : _v(0) {
    if (val < 0) {
      long long k = (std::abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  constexpr ModInt operator-() const { return ModInt(umod() - _v); }
  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long)(_v)*a._v) % M;
    return *this;
  }
  constexpr ModInt pow(unsigned long long t) const {
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  constexpr ModInt inv() const {
    // Inverse by Extended Euclidean algorithm.
    // M doesn't need to be prime, but x and M must be coprime.
    assert(_v != 0);
    auto [g, x, y] = ext_gcd(_v, M);
    assert(g == 1);  // The GCD must be 1.
    return x;

    // Inverse by Fermat's little theorem. M must be prime.
    //
    //     return pow(M - 2);
  }
  constexpr ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    return ModInt(a) /= b;
  }
  friend constexpr bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend constexpr bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  // Extended Euclidean algorithm.
  // Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
  static constexpr std::array<long long, 3> ext_gcd(int a, int b) {
    if (b == 0) return {a, 1, 0};
    auto res = ext_gcd(b, a % b);  // = (g, x, y)
    res[1] -= (a / b) * res[2];
    std::swap(res[1], res[2]);
    return res;  // = (g, y, x - (a/b)*y)
  }

  unsigned _v;  // raw value
};
const unsigned MOD = int(1e9) + 7;
using Mint = ModInt<MOD>;

// range-apply point-get.
template <typename Monoid>
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

 private:
  int size_;
  std::vector<T> data_;
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const DualSegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size(); ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}

struct Sum {
  using T = Mint;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

// tour: preorder node ids
// The interval [begin[v], end[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)
  vector<int> begin;  // index in the tour on entering the subtree of v
  vector<int> end;    // index in the tour on exiting the subtree of v
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
    tour.emplace_back(depth[v], v);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
      // tour.emplace_back(depth[v], v);
    }
    end[v] = int(tour.size());
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  vector<int> par(n, -1);
  vector<vector<int>> g(n);
  REP(v, 1, n) {
    int p;
    cin >> p;
    --p;
    g[v].push_back(p);
    g[p].push_back(v);
    par[v] = p;
  }

  EulerTour et(g);
  assert(ssize(et.tour) == n);
  DualSegTree<Sum> seg0(n), seg1(n);

  int q;
  cin >> q;
  REP(_, q) {
    i64 qt, v, x, k;
    cin >> qt >> v;
    --v;
    if (qt == 1) {
      cin >> x >> k;
      int l = et.begin[v], r = et.end[v];
      seg0.apply(l, r, Mint(x) + et.depth[v] * Mint(k));
      seg1.apply(l, r, -Mint(k));
    } else {
      const int j = et.begin[v];
      Mint ans = seg0[j] + Mint(et.depth[v]) * seg1[j];
      cout << ans << "\n";
    }
  }
}
