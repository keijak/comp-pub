#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/convolution>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename T, int DMAX>
struct NTTMult {
  static_assert(atcoder::internal::is_modint<T>::value, "Requires ACL modint.");
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");

  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<T> res = atcoder::convolution(x, y);
    if (int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x) {
    assert(x[0].val() != 0);  // must be invertible
    const int n = x.size();
    std::vector<T> res(n);
    res[0] = x[0].inv();
    for (int i = 1; i < n; i <<= 1) {
      const int m = std::min(2 * i, n);
      std::vector<T> f(2 * i), g(2 * i);
      for (int j = 0; j < m; ++j) f[j] = x[j];
      for (int j = 0; j < i; ++j) g[j] = res[j];
      f = atcoder::convolution(f, g);
      f.resize(2 * i);
      for (int j = 0; j < i; ++j) f[j] = 0;
      f = atcoder::convolution(f, g);
      for (int j = i; j < m; ++j) res[j] = -f[j];
    }
    return res;
  }
};

// Formal Power Series (dense format).
template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr int dmax() { return Mult::dmax(); }

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1, 0) {}  // = 0 * x^0

  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }
  DenseFPS(std::initializer_list<T> init) : coeff_(init.begin(), init.end()) {
    while (size() > dmax() + 1) coeff_.pop_back();
    assert(size() > 0);
  }

  DenseFPS(const DenseFPS &other) : coeff_(other.coeff_) {}
  DenseFPS(DenseFPS &&other) : coeff_(std::move(other.coeff_)) {}
  DenseFPS &operator=(const DenseFPS &other) {
    coeff_ = other.coeff_;
    return *this;
  }
  DenseFPS &operator=(DenseFPS &&other) {
    coeff_ = std::move(other.coeff_);
    return *this;
  }

  // size <= dmax + 1
  inline int size() const { return static_cast<int>(coeff_.size()); }

  // Returns the coefficient of x^k.
  inline T operator[](int k) const { return (k >= size()) ? 0 : coeff_[k]; }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const T &scalar) {
    return DenseFPS(x) += scalar;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(x) -= y;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(x) -= y;
  }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const T &scalar) {
    return DenseFPS(x) *= scalar;
  }
  friend DenseFPS operator*(const T &scalar, const DenseFPS &y) {
    return DenseFPS{scalar} *= y;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    return *this =
               DenseFPS(Mult::multiply(std::move(this->coeff_), other.coeff_));
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return DenseFPS(Mult::multiply(x.coeff_, y.coeff_));
  }

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x: coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &x, const T &scalar) {
    return DenseFPS(x) /= scalar;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &y) {
    return DenseFPS{scalar} /= y;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= DenseFPS(Mult::invert(other.coeff_));
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * DenseFPS(Mult::invert(y.coeff_));
  }

  DenseFPS pow(i64 t) const {
    assert(t >= 0);
    DenseFPS res = {1}, base = *this;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // Multiplies by (1 + c * x^k).
  void multiply2_inplace(int k, int c) {
    assert(k > 0);
    if (size() <= dmax()) {
      coeff_.resize(min(size() + k, dmax() + 1), 0);
    }
    for (int i = size() - 1; i >= k; --i) {
      coeff_[i] += coeff_[i - k] * c;
    }
  }
  // Multiplies by (1 + c * x^k).
  DenseFPS multiply2(int k, int c) const {
    DenseFPS res = *this;
    res.multiply2_inplace(k, c);
    return res;
  }

  // Divides by (1 + c * x^k).
  void divide2_inplace(int k, int c) {
    assert(k > 0);
    for (int i = k; i < size(); ++i) {
      coeff_[i] -= coeff_[i - k] * c;
    }
  }
  // Divides by (1 + c * x^k).
  DenseFPS divide2(int k, int c) const {
    DenseFPS res = *this;
    res.divide2_inplace(k, c);
    return res;
  }

  T eval(const T &a) const {
    T res = 0, x = 1;
    for (auto c: coeff_) {
      res += c * x;
      x *= a;
    }
    return res;
  }
};

// Formal Power Series (sparse format).
template<typename T>
struct SparseFPS {
  int size_;
  std::vector<int> degree_;
  std::vector<T> coeff_;

  SparseFPS() : size_(0) {}

  explicit SparseFPS(std::vector<std::pair<int, T>> terms)
      : size_(terms.size()), degree_(size_), coeff_(size_) {
    // Sort by degree.
    std::sort(terms.begin(), terms.end(),
              [](const auto &x, const auto &y) { return x.first < y.first; });
    for (int i = 0; i < size_; ++i) {
      auto[d, c] = terms[i];
      assert(d >= 0);
      degree_[i] = d;
      coeff_[i] = c;
    }
  }

  SparseFPS(std::initializer_list<std::pair<int, T>> terms)
      : SparseFPS(std::vector<std::pair<int, T>>(terms.begin(), terms.end())) {}

  inline int size() const { return size_; }
  inline const T &coeff(int i) const { return coeff_[i]; }
  inline int degree(int i) const { return degree_[i]; }
  int max_degree() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int d, T c) {
    assert(d >= 0);
    if (not degree_.empty()) {
      assert(d > degree_.back());
    }
    degree_.push_back(std::move(d));
    coeff_.push_back(std::move(c));
    ++size_;
  }

  // Returns the coefficient of x^d.
  T operator[](int d) const {
    auto it = std::lower_bound(degree_.begin(), degree_.end(), d);
    if (it == degree_.end() or *it != d) return (T) (0);
    int j = std::distance(degree_.begin(), it);
    return coeff_[j];
  }

  SparseFPS &operator+=(const T &scalar) {
    for (auto &x: coeff_) x += scalar;
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &x, const T &scalar) {
    SparseFPS res = x;
    res += scalar;
    return res;
  }
  SparseFPS &operator+=(const SparseFPS &other) {
    *this = this->add(other);
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &x, const SparseFPS &y) {
    return x.add(y);
  }

  SparseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff_) x *= scalar;
    return *this;
  }
  friend SparseFPS operator*(const SparseFPS &x, const T &scalar) {
    SparseFPS res = x;
    res *= scalar;
    return res;
  }

  SparseFPS &operator-=(const SparseFPS &other) {
    *this = this->add(other * -1);
    return *this;
  }
  friend SparseFPS operator-(const SparseFPS &x, const SparseFPS &y) {
    return x.add(y * -1);
  }

 private:
  SparseFPS add(const SparseFPS &other) const {
    SparseFPS res;
    int j = 0;  // two pointers (i, j)
    for (int i = 0; i < size(); ++i) {
      const int deg = this->degree(i);
      for (; j < other.size() and other.degree(j) < deg; ++j) {
        res.emplace_back(other.degree(j), other.coeff(j));
      }
      T c = this->coeff(i);
      if (j < other.size() and other.degree(j) == deg) {
        c += other.coeff(j);
        ++j;
      }
      if (c != 0) {
        res.emplace_back(deg, c);
      }
    }
    for (; j < other.size(); ++j) {
      res.emplace_back(other.degree(j), other.coeff(j));
    }
    return res;
  }
};

// Polynomial addition (dense + sparse).
template<typename FPS, typename T = typename FPS::T>
FPS &operator+=(FPS &x, const SparseFPS<T> &y) {
  for (int i = 0; i < y.size(); ++i) {
    if (y.degree(i) > FPS::dmax()) break;  // ignore
    x.coeff_[y.degree(i)] += y.coeff(i);
  }
  return x;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator+(const FPS &x, const SparseFPS<T> &y) {
  auto res = x;
  res += y;
  return res;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator+(const SparseFPS<T> &x, const FPS &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template<typename FPS, typename T = typename FPS::T>
FPS &operator*=(FPS &x, const SparseFPS<T> &y) {
  if (y.size() == 0) {
    return x *= 0;
  }
  const int yd_max = y.degree(y.size() - 1);
  const int xd_max = x.size() - 1;
  const int n = std::min(xd_max + yd_max, FPS::dmax()) + 1;
  if (x.size() < n) x.coeff_.resize(n);

  T c0 = 0;
  int j0 = 0;
  if (y.degree(0) == 0) {
    c0 = y.coeff(0);
    j0 = 1;
  }

  for (int xd = n - 1; xd >= 0; --xd) {
    x.coeff_[xd] *= c0;
    for (int j = j0; j < y.size(); ++j) {
      int yd = y.degree(j);
      if (yd > xd) break;
      x.coeff_[xd] += x[xd - yd] * y.coeff(j);
    }
  }
  return x;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator*(const FPS &x, const SparseFPS<T> &y) {
  auto res = x;
  res *= y;
  return res;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator*(const SparseFPS<T> &x, const FPS &y) {
  return y * x;  // commutative
}

constexpr int D = 3005;
using DF = DenseFPS<NTTMult<Mint, D>>;
using SF = SparseFPS<Mint>;

auto solve() {
  const int N = in, K = in;

  vector memo(N + 1, vector(N + 1, optional<Mint>()));

  auto f = [&](auto &f, int n, int k) -> Mint {
    if (n < k) return 0;
    if (k < 0) return 0;
    if (n < 0) return 0;
    if (n > 0 and k == 0) return 0;
    if (n == 0 and k == 0) return 1;
    optional<Mint> &memoized = memo[n][k];
    if (memoized.has_value()) return memoized.value();
    auto ret = f(f, n - 1, k - 1) + f(f, n, k * 2);
    memo[n][k] = ret;
    return ret;
  };

  return f(f, N, K);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
