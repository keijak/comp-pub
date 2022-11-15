// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/convolution>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

template<size_t BufSize>
class InputReader {
 public:
  void skip() {
    [[maybe_unused]] static const bool lazy_init = [this]() {
      const size_t len = fread(buf_, 1, sizeof(buf_) - 1, stdin);
      buf_[len] = '\0';
      ptr_ = buf_;
      bufend_ = buf_ + len;
      return true;
    }();
    while (isspace(*ptr_)) ++ptr_;
  }

  template<typename T>
  operator T() {
    T x;
    skip();
    assert(not is_eof());  // Couldn't read reached the end of input.
    read_one(x);
    return x;
  }

  struct Sized {
    InputReader<BufSize> &reader;
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        reader.skip();
        assert(not reader.is_eof());
        reader.read_one(x);
      }
      return xs;
    }
  };
  Sized operator()(int n) { return {*this, n}; }

  bool is_eof() const { return ptr_ >= bufend_; }

 private:
  template<class T>
  std::enable_if_t<std::is_integral_v<T>> read_one(T &x) {
    [[maybe_unused]] int sign = 1;
    if constexpr (std::is_signed_v<T>) {
      sign = (*ptr_ == '-') ? (++ptr_, -1) : 1;
    }
    x = 0;
    while (isdigit(*ptr_)) x = x * 10 + (*ptr_++ & 0x0F);
    if constexpr (std::is_signed_v<T>) {
      x *= sign;
    }
  }
  void read_one(std::string &s) {
    char *p0 = ptr_;
    while (not isspace(*ptr_)) ptr_++;
    s.assign(p0, ptr_);
  }
  void read_one(std::string_view &s) {
    const char *p0 = ptr_;
    while (not isspace(*ptr_)) ptr_++;
    s = std::string_view(p0, ptr_ - p0);
  }

  static inline char buf_[BufSize];
  char *ptr_, *bufend_;
};
InputReader<(1 << 26)> in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_() {
  // std::ios::sync_with_stdio(false);
  // std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  in.skip();
  assert(in.is_eof());  // Some input is left.
#endif
  fflush(stdout), fflush(stderr);
  std::cout.flush(), std::cerr.flush();
  std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

// T: modint
template<typename T, int DMAX>
struct NTTMult {
  static_assert(atcoder::internal::is_modint<T>::value, "Requires ACL modint.");
  static_assert(T::mod() == 998244353, "Requires an NTT-friendly mod.");
  static_assert(DMAX <= (1 << 23), "Too big degrees for NTT.");

  using value_type = T;
  static constexpr int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    std::vector<T> res = atcoder::convolution(x, y);
    if (int(res.size()) > DMAX + 1) res.resize(DMAX + 1);  // shrink
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x, int sz = -1) {
    int n = x.size();
    assert(n != 0 && x[0].val() != 0);  // must be invertible
    if (sz == -1) sz = dmax() + 1;
    assert(sz > 0);
    std::vector<T> res{x[0].inv()};
    for (int m = 1, m2 = 2; m < sz; m = m2, m2 *= 2) {
      std::vector<T> f(x.begin(), x.begin() + std::min(n, m2));
      std::vector<T> g(res);
      f.resize(m2), atcoder::internal::butterfly(f);
      g.resize(m2), atcoder::internal::butterfly(g);
      for (int i = 0; i < m2; ++i) f[i] *= g[i];
      atcoder::internal::butterfly_inv(f);
      f.erase(f.begin(), f.begin() + m);
      f.resize(m2), atcoder::internal::butterfly(f);
      for (int i = 0; i < m2; ++i) f[i] *= g[i];
      atcoder::internal::butterfly_inv(f);
      T iz = T(m2).inv();
      iz *= -iz;
      for (int i = 0; i < m; ++i) f[i] *= iz;
      res.insert(res.end(), f.begin(), f.begin() + m);
    }
    res.resize(sz);
    return res;
  }
};

template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr int dmax() { return Mult::dmax(); }

  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff;

  DenseFPS() : coeff(1, 0) {}  // zero

  explicit DenseFPS(std::vector<T> c) : coeff(std::move(c)) {
    while (size() > dmax() + 1) coeff.pop_back();
    assert(size() > 0);
  }
  DenseFPS(std::initializer_list<T> c) : coeff(c.begin(), c.end()) {
    while (size() > dmax() + 1) coeff.pop_back();
    assert(size() > 0);
  }

  // size <= dmax + 1
  inline int size() const { return static_cast<int>(coeff.size()); }

  // Returns the coefficient of x^k.
  inline const T operator[](int k) const {
    return (k >= size()) ? 0 : coeff[k];
  }

  // Removes trailing zeros.
  void shrink() {
    while (coeff.size() > 1 and coeff.back() == T(0)) coeff.pop_back();
  }

  // Convenience method for debug dump.
  std::vector<T> head(int n = 10) const {
    n = min<int>(n, coeff.size());
    return std::vector<T>(coeff.begin(), coeff.begin() + n);
  }

  DenseFPS &operator+=(const T &scalar) {
    coeff[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const T &scalar) {
    DenseFPS r = f;
    r += scalar;
    return r;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) coeff.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &f, const DenseFPS &g) {
    DenseFPS r = f;
    r += g;
    return r;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) coeff.resize(other.size());
    for (int i = 0; i < other.size(); ++i) coeff[i] -= other[i];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &f, const DenseFPS &g) {
    DenseFPS r = f;
    r -= g;
    return r;
  }

  DenseFPS operator-() const { return *this * -1; }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &f, const T &scalar) {
    DenseFPS r = f;
    r *= scalar;
    return r;
  }
  friend DenseFPS operator*(const T &scalar, const DenseFPS &g) {
    DenseFPS r{scalar};
    r *= g;
    return r;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    return *this =
               DenseFPS(Mult::multiply(std::move(this->coeff), other.coeff));
  }
  friend DenseFPS operator*(const DenseFPS &f, const DenseFPS &g) {
    return DenseFPS(Mult::multiply(f.coeff, g.coeff));
  }

  // Multiplies by x^k (with truncation).
  void shift_inplace(int k) {
    if (k > 0) {
      if (size() <= dmax()) {
        coeff.resize(std::min<int>(size() + k, dmax() + 1), 0);
      }
      for (int i = size() - 1; i >= k; --i) {
        coeff[i] = coeff[i - k];
      }
      for (int i = k - 1; i >= 0; --i) {
        coeff[i] = 0;
      }
    } else if (k < 0) {
      // If coefficients of degrees higher than dmax() were truncated
      // beforehand, you lose the information. Ensure dmax() is big enough.
      coeff.erase(coeff.begin(), coeff.begin() + std::min(-k, size()));
    }
  }
  // Multiplies by x^k.
  DenseFPS shift(int k) const {
    DenseFPS r = *this;
    r.shift_inplace(k);
    return r;
  }

  DenseFPS inv(int sz = -1) const { return DenseFPS(Mult::invert(coeff, sz)); }

  DenseFPS &operator/=(const T &scalar) {
    T d_inv = T(1) / scalar;
    for (auto &x: coeff) x *= d_inv;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &f, const T &scalar) {
    DenseFPS r = f;
    r /= scalar;
    return r;
  }
  friend DenseFPS operator/(const T &scalar, const DenseFPS &g) {
    DenseFPS r{scalar};
    r /= g;
    return r;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    int z = 0;
    const int msz = std::min<int>(size(), other.size());
    while (z < msz and (*this)[z] == T(0) and other[z] == T(0)) ++z;
    if (z == size()) {
      return *this;  // (0/y) == 0 regardless of y.
    }
    if (z == 0) {
      return *this *= DenseFPS(Mult::invert(other.coeff));
    } else {
      shift_inplace(-z);
      std::vector<T> y(other.coeff.begin() + std::min<int>(z, other.size()),
                       other.coeff.end());
      return *this *= DenseFPS(Mult::invert(std::move(y)));
    }
  }
  friend DenseFPS operator/(const DenseFPS &f, const DenseFPS &g) {
    DenseFPS r = f;
    r /= g;
    return r;
  }
};

// Formal Power Series (sparse format).
template<typename T>
struct SparseFPS {
  int size_;
  std::vector<int> degree;
  std::vector<T> coeff;

  SparseFPS() : size_(0) {}  // zero

  explicit SparseFPS(std::vector<std::pair<int, T>> terms)
      : size_(terms.size()), degree(size_), coeff(size_) {
    // Sort by degree.
    std::sort(terms.begin(), terms.end(),
              [](const auto &x, const auto &y) { return x.first < y.first; });
    for (int i = 0; i < size_; ++i) {
      auto [d, c] = terms[i];
      assert(d >= 0);
      degree[i] = d;
      coeff[i] = c;
    }
  }

  SparseFPS(std::initializer_list<std::pair<int, T>> terms)
      : SparseFPS(std::vector<std::pair<int, T>>(terms.begin(), terms.end())) {}

  inline int size() const { return size_; }
  int max_degree() const { return (size_ == 0) ? 0 : degree.back(); }

  void emplace_back(int d, T c) {
    assert(d >= 0);
    if (not degree.empty()) {
      assert(d > degree.back());
    }
    degree.push_back(std::move(d));
    coeff.push_back(std::move(c));
    ++size_;
  }

  // Returns the coefficient of x^d.
  const T operator[](int d) const {
    auto it = std::lower_bound(degree.begin(), degree.end(), d);
    if (it == degree.end() or *it != d) return (T) (0);
    int j = std::distance(degree.begin(), it);
    return coeff[j];
  }

  SparseFPS &operator+=(const T &scalar) {
    for (auto &x: coeff) x += scalar;
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &f, const T &scalar) {
    SparseFPS res = f;
    res += scalar;
    return res;
  }
  SparseFPS &operator+=(const SparseFPS &other) {
    *this = this->add(other);
    return *this;
  }
  friend SparseFPS operator+(const SparseFPS &f, const SparseFPS &g) {
    return f.add(g);
  }

  SparseFPS &operator*=(const T &scalar) {
    for (auto &x: coeff) x *= scalar;
    return *this;
  }
  friend SparseFPS operator*(const SparseFPS &f, const T &scalar) {
    SparseFPS res = f;
    res *= scalar;
    return res;
  }

  SparseFPS &operator-=(const SparseFPS &other) {
    *this = this->add(other * -1);
    return *this;
  }
  friend SparseFPS operator-(const SparseFPS &f, const SparseFPS &g) {
    return f.add(g * -1);
  }

 private:
  SparseFPS add(const SparseFPS &other) const {
    SparseFPS res;
    int j = 0;  // two pointers (i, j)
    for (int i = 0; i < size(); ++i) {
      const int deg = this->degree[i];
      for (; j < other.size() and other.degree[j] < deg; ++j) {
        res.emplace_back(other.degree[j], other.coeff[j]);
      }
      T c = this->coeff[i];
      if (j < other.size() and other.degree[j] == deg) {
        c += other.coeff[j];
        ++j;
      }
      if (c != 0) {
        res.emplace_back(deg, c);
      }
    }
    for (; j < other.size(); ++j) {
      res.emplace_back(other.degree[j], other.coeff[j]);
    }
    return res;
  }
};

// Polynomial addition (dense + sparse).
template<typename FPS, typename T = typename FPS::T>
FPS &operator+=(FPS &f, const SparseFPS<T> &g) {
  for (int i = 0; i < g.size(); ++i) {
    if (g.degree[i] > FPS::dmax()) break;  // ignore
    f.coeff[g.degree[i]] += g.coeff[i];
  }
  return f;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator+(const FPS &f, const SparseFPS<T> &g) {
  auto res = f;
  res += g;
  return res;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator+(const SparseFPS<T> &f, const FPS &g) {
  return g + f;  // commutative
}

// Polynomial multiplication (dense * sparse). O(NM).
template<typename FPS, typename T = typename FPS::T>
FPS &operator*=(FPS &f, const SparseFPS<T> &g) {
  if (g.size() == 0) {
    return f *= 0;
  }
  const int gd_max = g.degree[g.size() - 1];
  const int fd_max = f.size() - 1;
  const int n = std::min<int>(fd_max + gd_max, FPS::dmax()) + 1;
  if (f.size() < n) f.coeff.resize(n);

  T c0 = 0;
  int j0 = 0;
  if (g.degree[0] == 0) {
    c0 = g.coeff[0];
    j0 = 1;
  }

  for (int fd = n - 1; fd >= 0; --fd) {
    f.coeff[fd] *= c0;
    for (int j = j0; j < g.size(); ++j) {
      int gd = g.degree[j];
      if (gd > fd) break;
      f.coeff[fd] += f[fd - gd] * g.coeff[j];
    }
  }
  return f;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator*(const FPS &f, const SparseFPS<T> &g) {
  auto res = f;
  res *= g;
  return res;
}
template<typename FPS, typename T = typename FPS::T>
FPS operator*(const SparseFPS<T> &f, const FPS &g) {
  return g * f;  // commutative
}

using DF = DenseFPS<NTTMult<Mint, 3005>>;
using SF = SparseFPS<Mint>;

auto solve() {
  const int N = in, S = in;
  vector<int> A = in(N);
  DF f = {1};
  REP(i, N) {
    f *= SF{{0, 2}, {A[i], 1}};
  }
  out(f[S]);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
