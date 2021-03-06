#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
bool chmax(T &a, T b) {
  return a >= b ? false : ((a = std::move(b)), true);
}
template <class T>
bool chmin(T &a, T b) {
  return a <= b ? false : ((a = std::move(b)), true);
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

#ifdef ENABLE_DEBUG
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

const int MOD = 998244353;

struct mint {
  int x;

  constexpr mint(long long val = 0) : x(0) {
    if (val < 0) {
      long long k = (abs(val) + MOD - 1) / MOD;
      val += k * MOD;
    }
    assert(val >= 0);
    this->x = val % MOD;
  }
  constexpr mint operator-() const { return mint(-x); }
  constexpr mint &operator+=(const mint &a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  constexpr mint &operator-=(const mint &a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  constexpr mint &operator*=(const mint &a) {
    x = (x * (long long)a.x) % MOD;
    return *this;
  }
  constexpr mint pow(long long t) const {
    assert(t >= 0);
    mint base = *this;
    mint res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }
  // for prime MOD
  constexpr mint inv() const { return pow(MOD - 2); }
  constexpr mint &operator/=(const mint &a) { return *this *= a.inv(); }
};
constexpr mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
constexpr mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
constexpr mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
constexpr mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
constexpr bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
constexpr bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
constexpr bool operator<(const mint &a, const mint &b) { return a.x < b.x; }
constexpr bool operator>(const mint &a, const mint &b) { return a.x > b.x; }
constexpr bool operator<=(const mint &a, const mint &b) { return a.x <= b.x; }
constexpr bool operator>=(const mint &a, const mint &b) { return a.x >= b.x; }
istream &operator>>(istream &is, mint &a) { return is >> a.x; }
ostream &operator<<(ostream &os, const mint &a) { return os << a.x; }

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;
  int size;
  vector<T> dat;

  SegTree(int n) : size(n), dat(2 * n, Monoid::unity()) {}

  // Sets i-th value (0-indexed) to x for initial setup.
  // build() must be called after set() calls.
  void set(int i, const T &x) { dat[size + i] = x; }
  void build() {
    for (int k = size - 1; k > 0; --k) {
      dat[k] = Monoid::op(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const T &x) {
    int k = size + i;
    dat[k] = x;
    while (k > 1) {
      k >>= 1;
      dat[k] = Monoid::op(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, open interval).
  T fold(int l, int r) {
    l += size;
    r += size;
    T vleft = Monoid::unity(), vright = Monoid::unity();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, dat[l++]);
      if (r & 1) vright = Monoid::op(dat[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  T operator[](int i) const { return dat[size + i]; }
};
template <typename T>
ostream &operator<<(ostream &os, const SegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size; ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}

struct AddOp {
  using T = mint;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T unity() { return 0; }
};

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  V<pair<int, int>> lr(k);
  REP(i, k) {
    int l, r;
    cin >> l >> r;
    lr[i] = {l, r};
  }

  // 貰うDP
  SegTree<AddOp> st(n + 1);
  st.update(1, 1);
  for (int i = 2; i <= n; ++i) {
    REP(j, k) {
      auto [l, r] = lr[j];
      if (i - l < 0) continue;
      st.update(i, st[i] + st.fold(max(i - r, 0), i - l + 1));
    }
  }
  cout << st[n] << endl;
}
