#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>

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
using mint = atcoder::modint998244353;

// Formal Power Series (dense format).
template <typename T, int DMAX>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int)c.size() <= DMAX + 1);
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

  int size() const { return (int)coeff_.size(); }

  // Returns the coefficient of x^dy.
  T operator[](int dy) const {
    if (dy >= size()) return 0;
    return coeff_[dy];
  }

  DenseFPS &operator+=(const T &scalar) {
    coeff_[0] += scalar;
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res += scalar;
    return res;
  }
  DenseFPS &operator+=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int dx = 0; dx < other.size(); ++dx) coeff_[dx] += other[dx];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res += y;
    return res;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    if (size() < other.size()) {
      coeff_.resize(other.size());
    }
    for (int dx = 0; dx < other.size(); ++dx) coeff_[dx] -= other[dx];
    return *this;
  }
  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res -= y;
    return res;
  }

  DenseFPS &operator*=(const T &scalar) {
    for (auto &x : coeff_) x *= scalar;
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res *= scalar;
    return res;
  }
  DenseFPS &operator*=(const DenseFPS &other) {
    *this = this->mul_naive(other);
    return *this;
  }
  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    return x.mul_naive(y);
  }

  // Naive multiplication. O(N^2)
  DenseFPS inv() const {
    std::vector<T> res(size());
    res[0] = coeff_[0].inv();
    for (int i = 1; i < size(); ++i) {
      mint s = 0;
      for (int j = 1; j <= i; ++j) {
        s += coeff_[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return DenseFPS(std::move(res));
  }

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    const int n = std::min(size() + other.size() - 1, DMAX + 1);
    std::vector<T> res(n);
    for (int dx = 0; dx < size(); ++dx) {
      for (int j = 0; j < other.size(); ++j) {
        if (dx + j >= n) break;
        res[dx + j] += coeff_[dx] * other.coeff_[j];
      }
    }
    return DenseFPS(std::move(res));
  }
};

template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> mul_ntt(const DenseFPS<ModInt, DMAX> &x,
                               const DenseFPS<ModInt, DMAX> &y) {
  static_assert(ModInt::mod() != 1'000'000'007);  // Must be a NTT-friendly MOD!
  auto res = atcoder::convolution(x.coeff_, y.coeff_);
  if (res.size() > DMAX + 1) res.resize(DMAX + 1);  // shrink
  return DenseFPS<ModInt, DMAX>(std::move(res));
}

template <typename T, int DMAX>
DenseFPS<T, DMAX> pow_generic(
    const DenseFPS<T, DMAX> &x, u64 t,
    DenseFPS<T, DMAX> (*mulfunc)(const DenseFPS<T, DMAX> &,
                                 const DenseFPS<T, DMAX> &)) {
  DenseFPS<T, DMAX> base = x, res;
  res.coeff_[0] = 1;
  while (t) {
    if (t & 1) res = mulfunc(res, base);
    base = mulfunc(base, base);
    t >>= 1;
  }
  return res;
}

template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> pow_ntt(const DenseFPS<ModInt, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_ntt);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<mint> a(n);
  REP(i, n) {
    int x;
    cin >> x;
    a[i] = x;
  }

  DenseFPS<mint, 500000> f(std::move(a));
  auto g = pow_ntt(f, m);
  REP(i, n) {
    if (i > 0) cout << ' ';
    cout << g[i].val();
  }
  cout << endl;
}
