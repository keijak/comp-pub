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

// Formal Power Series (dense format).
template <typename T, int DMAX>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^DMAX.
  std::vector<T> coeff_;

  DenseFPS() : coeff_(DMAX + 1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff_(std::move(c)) {
    assert((int)coeff_.size() == DMAX + 1);
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

  static constexpr int size() { return DMAX + 1; }

  // Returns the coefficient of x^d.
  const T &operator[](int d) const { return coeff_[d]; }

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
    for (int i = 0; i < size(); ++i) coeff_[i] += other[i];
    return *this;
  }
  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res = x;
    res += y;
    return res;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) coeff_[i] -= other[i];
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

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    DenseFPS res;
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; i + j < size(); ++j) {
        res.coeff_[i + j] += (*this)[i] * other[j];
      }
    }
    return res;
  }
};

namespace fps {

// Fast polynomial multiplication by single NTT.
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> mul_ntt(const DenseFPS<ModInt, DMAX> &x,
                               const DenseFPS<ModInt, DMAX> &y) {
  static_assert(ModInt::mod() != 1'000'000'007);  // Must be a NTT-friendly MOD!
  auto z = atcoder::convolution(x.coeff_, y.coeff_);
  z.resize(DMAX + 1);  // Maybe shrink.
  return {std::move(z)};
}

// Polynomial multiplication by NTT + Garner (arbitrary mod).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> mul_mod(const DenseFPS<ModInt, DMAX> &x,
                               const DenseFPS<ModInt, DMAX> &y) {
  std::vector<i64> xll(x.size()), yll(y.size());
  for (int i = 0; i < x.size(); ++i) {
    xll[i] = x[i].val();
  }
  for (int i = 0; i < y.size(); ++i) {
    yll[i] = y[i].val();
  }
  auto zll = atcoder::convolution_ll(xll, yll);
  DenseFPS<ModInt, DMAX> res;
  int n = std::min<int>(res.size(), zll.size());
  for (int i = 0; i < n; ++i) {
    res.coeff_[i] = zll[i];
  }
  return res;
}

// Polynomial multiplication by NTT + Garner (long long).
template <int DMAX>
DenseFPS<i64, DMAX> mul_ll(const DenseFPS<i64, DMAX> &x,
                           const DenseFPS<i64, DMAX> &y) {
  auto z = atcoder::convolution_ll(x.coeff_, y.coeff_);
  z.resize(DMAX + 1);  // Maybe shrink.
  return DenseFPS<i64, DMAX>(std::move(z));
}

template <typename T, int DMAX, typename Func>
DenseFPS<T, DMAX> pow_generic(const DenseFPS<T, DMAX> &x, u64 t, Func mulfunc) {
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

template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> pow_mod(const DenseFPS<ModInt, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_mod);
}

template <int DMAX>
DenseFPS<i64, DMAX> pow_ll(const DenseFPS<i64, DMAX> &x, u64 t) {
  return pow_generic(x, t, mul_ll);
}

}  // namespace fps

// Formal Power Series (sparse format).
template <typename T>
struct SparseFPS {
  int size_;
  std::vector<int> degree_;
  std::vector<T> coeff_;

  SparseFPS() : size_(0) {}

  explicit SparseFPS(std::vector<std::pair<int, T>> terms)
      : size_(terms.size()), degree_(size_), coeff_(size_) {
    // Sort by degree_ in ascending order.
    sort(terms.begin(), terms.end());
    for (int i = 0; i < size_; ++i) {
      degree_[i] = terms[i].first;
      coeff_[i] = terms[i].second;
    }
  }

  inline int size() const { return size_; }
  inline int degree(int i) const { return degree_[i]; }
  inline const T &coeff(int i) const { return coeff_[i]; }

  int DMAX() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int d, T c) {
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
    if (it == degree_.end() or *it != d) return (T)(0);
    int j = std::distance(degree_.begin(), it);
    return coeff_[j];
  }

  SparseFPS &operator+=(const T &scalar) {
    for (auto &x : coeff_) x += scalar;
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
    for (auto &x : coeff_) x *= scalar;
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
    int j = 0;
    for (int i = 0; i < size();) {
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
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> &operator+=(DenseFPS<ModInt, DMAX> &x,
                                   const SparseFPS<ModInt> &y) {
  for (int i = 0; i < y.size(); ++i) {
    if (y.degree(i) > DMAX) break;  // ignore
    x.coeff_[y.degree(i)] += y.coeff(i);
  }
  return x;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator+(const DenseFPS<ModInt, DMAX> &x,
                                 const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, DMAX> res = x;
  res += y;
  return res;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator+(const SparseFPS<ModInt> &x,
                                 const DenseFPS<ModInt, DMAX> &y) {
  return y + x;  // commutative
}

// Polynomial multiplication (dense * sparse).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> &operator*=(DenseFPS<ModInt, DMAX> &x,
                                   const SparseFPS<ModInt> &y) {
  if (y.size() == 0) {
    return x *= 0;
  }
  ModInt c0 = 0;
  int j0 = 0;
  if (y.degree(0) == 0) {
    c0 = y.coeff(0);
    ++j0;
  }
  for (int i = DMAX; i >= 0; --i) {
    x.coeff_[i] *= c0;
    for (int j = j0; j < y.size(); ++j) {
      int d = y.degree(j);
      if (d > i) break;
      x.coeff_[i] += x[i - d] * y.coeff(j);
    }
  }
  return x;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator*(const DenseFPS<ModInt, DMAX> &x,
                                 const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, DMAX> res = x;
  res *= y;
  return res;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator*(const SparseFPS<ModInt> &x,
                                 const DenseFPS<ModInt, DMAX> &y) {
  return y * x;  // commutative
}

// Polynomial division (dense / sparse).
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> &operator/=(DenseFPS<ModInt, DMAX> &x,
                                   const SparseFPS<ModInt> &y) {
  assert(y.size() > 0 and y.degree(0) == 0 and y.coeff(0) != 0);
  ModInt inv_c0 = y.coeff(0).inv();
  for (int i = 0; i < x.size(); ++i) {
    for (int j = 1; j < y.size(); ++j) {
      int d = y.degree(j);
      if (d > i) break;
      x.coeff_[i] -= x.coeff_[i - d] * y.coeff[j];
    }
    x.coeff_[i] *= inv_c0;
  }
  return x;
}
template <typename ModInt, int DMAX>
DenseFPS<ModInt, DMAX> operator/(const DenseFPS<ModInt, DMAX> &x,
                                 const SparseFPS<ModInt> &y) {
  DenseFPS<ModInt, DMAX> res = x;
  res /= y;
  return res;
}

using mint = atcoder::modint998244353;
const int MOD = 998244353;
const int BMAX = 6000;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<int> A(N);
  cin >> A;

  DenseFPS<mint, BMAX> f;
  f += 1;
  REP(i, N) {
    SparseFPS<mint> g;
    g.emplace_back(0, mint(A[i] - 1));
    g.emplace_back(1, mint(1));
    f *= move(g);
  }

  REP(i, Q) {
    int b;
    cin >> b;
    cout << f[b].val() << '\n';
  }
}