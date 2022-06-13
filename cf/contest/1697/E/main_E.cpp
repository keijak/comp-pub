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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
void test_case(int t, int T) {
  if (T <= 1) return;
  std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
            << std::endl;
}
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define test_case(...)
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<unsigned M>
struct ModInt {
  constexpr ModInt() : _v(0) {}
  constexpr ModInt(long long val) {
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
    _v = ((unsigned long long) (_v) * a._v) % M;
    return *this;
  }
  constexpr ModInt pow(long long t) const {
    if (_v == 0) {
      return 0;  // corner case: 0^0 = ?
    }
    if (t < 0) {
      return this->inv().pow(-t);
    }
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // https://qiita.com/Mitarushi/items/8d7fb52e8a80e8008463
  constexpr ModInt inv() const {
    long long b = 1, a = _v;
    while (a > 1) {
      long long q = M / a;
      a = M - a * q;
      b = -b * q % M;
    }
    assert(a == 1);  // if a = 0, _v and M are not coprime.
    if (b < 0) b += M;
    ModInt ret;
    ret._v = (unsigned) b;
    return ret;
  }
  constexpr ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r += b;
    return r;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r -= b;
    return r;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r *= b;
    return r;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r /= b;
    return r;
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
  unsigned _v;  // raw value
};
const unsigned MOD = 998244353;
using Mint = ModInt<MOD>;

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    int ub = std::upper_bound(values.begin(), values.end(), x) - values.begin();
    return ub - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

// mod: prime
template<class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  explicit Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (binomial coefficients)
  T C(Int n, Int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
  // Permutation (nPk)
  T P(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[n - k];
  }
};

using P = array<int, 2>;

Int get_distance(const P &a, const P &b) {
  return std::abs(a[0] - b[0]) + std::abs(a[1] - b[1]);
}

Mint solve() {
  const int n = in;
  vector<P> pos(n);
  REP(i, n) {
    int x = in, y = in;
    pos[i] = {x, y};
  }
  vector<Int> mindist(n, kBigVal<Int>);
  REP(i, n) {
    REP(j, n) {
      if (i == j) continue;
      chmin(mindist[i], get_distance(pos[i], pos[j]));
    }
  }
  DUMP(mindist);

  UnionFind uf(n + 1);
  const int dirty = n;
  REP(i, n) {
    REP(j, n) {
      if (i == j) continue;
      Int d = get_distance(pos[i], pos[j]);
      assert(d >= mindist[i]);
      assert(d >= mindist[j]);
      if (d == mindist[i] and d == mindist[j]) {
        uf.unite(i, j);
      }
      if (d == mindist[i] and mindist[j] < d) {
        uf.unite(i, dirty);
      }
    }
  }

  map<int, vector<int>> groups;
  const int dirty_group = uf.find(dirty);
  REP(v, n) {
    int g = uf.find(v);
    if (g != dirty_group) {
      groups[g].push_back(v);
    }
  }
  int alone_count = uf.size(dirty_group) - 1;
  vector<int> valid_group_sizes;
  for (const auto &[g, vs]: groups) {
    bool valid = false;
    if (vs.size() >= 2) {
      valid = [&](const vector<int> &xs) -> bool {
        Int md = mindist[xs[0]];
        REP(i, 1, xs.size()) {
          if (mindist[xs[i]] != md) return false;
          REP(j, i) {
            Int d = get_distance(pos[xs[i]], pos[xs[j]]);
            if (d != md) {
              return false;
            }
          }
        }
        return true;
      }(vs);
    }
    if (valid) {
      Int d = mindist[vs[0]];
      DUMP(g, vs, d);
      valid_group_sizes.push_back(ssize(vs));
    } else {
      alone_count += ssize(vs);
    }
  }
  DUMP(valid_group_sizes);
  const int M = ssize(valid_group_sizes);

  auto dp = vector(M + 1, vector(n + 1, Mint(0)));
  dp[0][alone_count] = 1;
  REP(i, M) {
    int sz = valid_group_sizes[i];
    REP(j, n + 1) {
      if (dp[i][j].val() == 0) continue;
      if (j + 1 <= n) dp[i + 1][j + 1] += dp[i][j];
      if (j + sz <= n) dp[i + 1][j + sz] += dp[i][j];
    }
  }
  DUMP(dp[M]);
  Factorials<Mint> fs(n + 1);
  Mint ans = 0;
  REP(k, n + 1) {
    ans += fs.P(n, k) * dp[M][k];
  }
  return ans;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    print(solve());
  }
  exit_();
}
