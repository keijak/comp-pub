// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
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

template<typename T, Int DMAX>
struct NaiveMult {
  using value_type = T;
  static constexpr Int dmax() { return DMAX; }

  static std::vector<T> multiply(const std::vector<T> &x,
                                 const std::vector<T> &y) {
    const int n = std::min<int>(x.size() + y.size() - 1, DMAX + 1);
    const int mi = std::min<int>(x.size(), n);
    std::vector<T> res(n);
    for (int i = 0; i < mi; ++i) {
      for (int j = 0; j < int(y.size()); ++j) {
        if (i + j >= n) break;
        res[i + j] += x[i] * y[j];
      }
    }
    return res;
  }

  static std::vector<T> invert(const std::vector<T> &x, Int sz = -1) {
    if (sz == -1) sz = dmax() + 1;
    std::vector<T> res(sz);
    res[0] = T(1) / x[0];
    for (int i = 1; i < sz; ++i) {
      T s = 0;
      const int mj = std::min<int>(i + 1, x.size());
      for (int j = 1; j < mj; ++j) {
        s += x[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return res;
  }
};

// Formal Power Series (dense format).
template<typename Mult>
struct DenseFPS {
  using T = typename Mult::value_type;
  static constexpr Int dmax() { return Mult::dmax(); }

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
        coeff.resize(std::min<Int>(size() + k, dmax() + 1), 0);
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
    const int msz = std::min(size(), other.size());
    while (z < msz and (*this)[z] == T(0) and other[z] == T(0)) ++z;
    if (z == size()) {
      return *this;  // (0/y) == 0 regardless of y.
    }
    if (z == 0) {
      return *this *= DenseFPS(Mult::invert(other.coeff));
    } else {
      shift_inplace(-z);
      std::vector<T> y(other.coeff.begin() + std::min(z, other.size()),
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

template<typename FPS>
void multiply2_inplace(FPS &f, int c, int k) {
  assert(k > 0);
  if (f.size() <= FPS::dmax()) {
    f.coeff.resize(std::min<int>(f.size() + k, FPS::dmax() + 1), 0);
  }
  for (int i = f.size() - 1; i >= k; --i) {
    f.coeff[i] += f.coeff[i - k] * c;
  }
}

template<typename FPS>
void divide2_inplace(FPS &f, int c, int k) {
  assert(k > 0);
  for (int i = k; i < f.size(); ++i) {
    f.coeff[i] -= f.coeff[i - k] * c;
  }
}

template<typename FPS>
typename FPS::T bostan_mori(FPS P, FPS Q, long long n) {
  assert(FPS::dmax() >= std::max(P.size(), Q.size()) * 2);
  typename FPS::T ret = 0;
  P.coeff.resize(Q.size() - 1);
  while (n) {
    auto Q2 = Q;
    for (int i = 1; i < Q2.size(); i += 2) {
      Q2.coeff[i] = -Q2[i];
    }
    const auto S = P * Q2;
    const auto T = Q * Q2;
    for (int i = int(n & 1); i < S.size(); i += 2) {
      P.coeff[i >> 1] = S[i];
    }
    for (int i = 0; i < T.size(); i += 2) {
      Q.coeff[i >> 1] = T[i];
    }
    n >>= 1;
  }
  ret += P[0];
  return ret;
}

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

using DF = DenseFPS<NaiveMult<Mint, 44>>;
using SF = SparseFPS<Mint>;

int main() {
  init_();

  DF P = {1};
  P.shift_inplace(5);
  DF Q = {1};
  REP(i, 5) Q *= SF{{0, 1}, {2, -1}};
  REP(i, 11) Q *= SF{{0, 1}, {1, -1}};

  const int T = in;
  REP(t, T) {
    test_case(t, T);
    const int N = in;
    Mint ans = bostan_mori(P, Q, N);
    out(ans);
  }
  exit_();
}
