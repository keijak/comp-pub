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

#ifdef MY_DEBUG
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

  // Returns the coefficient of x^d.
  T operator[](int d) const {
    if (d >= size()) return 0;
    return coeff_[d];
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
    for (int i = 0; i < other.size(); ++i) coeff_[i] += other[i];
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
    for (int i = 0; i < other.size(); ++i) coeff_[i] -= other[i];
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
    const int n = min(size() + other.size() - 1, DMAX + 1);
    DenseFPS res;
    res.coeff_.resize(n);
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; j < other.size(); ++j) {
        if (i + j >= n) break;
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
  if (z.size() > DMAX + 1) {
    z.resize(DMAX + 1);
  }
  return DenseFPS<ModInt, DMAX>(std::move(z));
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
    for (int dx = 0; dx < size_; ++dx) {
      degree_[dx] = terms[dx].first;
      coeff_[dx] = terms[dx].second;
    }
  }

  inline int size() const { return size_; }
  inline int degree(int dx) const { return degree_[dx]; }
  inline const T &coeff(int dx) const { return coeff_[dx]; }

  int DMAX() const { return (size_ == 0) ? 0 : degree_.back(); }

  void emplace_back(int dy, T c) {
    if (not degree_.empty()) {
      assert(dy > degree_.back());
    }
    degree_.push_back(std::move(dy));
    coeff_.push_back(std::move(c));
    ++size_;
  }

  // Returns the coefficient of x^dy.
  T operator[](int dy) const {
    auto it = std::lower_bound(degree_.begin(), degree_.end(), dy);
    if (it == degree_.end() or *it != dy) return (T)(0);
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
    for (int dx = 0; dx < size();) {
      const int deg = this->degree(dx);
      for (; j < other.size() and other.degree(j) < deg; ++j) {
        res.emplace_back(other.degree(j), other.coeff(j));
      }
      T c = this->coeff(dx);
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
  for (int dx = 0; dx < y.size(); ++dx) {
    if (y.degree(dx) > DMAX) break;  // ignore
    x.coeff_[y.degree(dx)] += y.coeff(dx);
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
  const int dy_max = y.degree(y.size() - 1);
  const int dx_max = x.size() - 1;
  const int n = std::min(dx_max + dy_max + 1, DMAX + 1);
  if (x.size() < n) x.coeff_.resize(n);
  for (int dx = n - 1; dx >= 0; --dx) {
    x.coeff_[dx] *= c0;
    for (int j = j0; j < y.size(); ++j) {
      int dy = y.degree(j);
      if (dy > dx) break;
      x.coeff_[dx] += x[dx - dy] * y.coeff(j);
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

using mint = atcoder::modint998244353;
const int MOD = 998244353;
const int PMAX = 6'000;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<int> A(N);
  cin >> A;
  sort(ALL(A));
  V<DenseFPS<mint, PMAX>> ps(N + 1);
  V<mint> prods(N + 1);
  prods[0] = 1;
  REP(i, N) prods[i + 1] = prods[i] * A[i];

  DenseFPS<mint, PMAX> f;
  f += 1;
  ps[N] = f;

  REP(i, N) {
    int ri = N - 1 - i;
    SparseFPS<mint> g;
    g.emplace_back(0, A[ri] - 1);
    g.emplace_back(1, 1);
    ps[ri] = ps[ri + 1] * move(g);
  }

  REP(i, Q) {
    int l, r, p;
    cin >> l >> r >> p;

    u64 ans = 0;
    for (int x = l; x <= r; ++x) {
      int j = lower_bound(A.begin(), A.end(), x) - A.begin();
      mint count = ps[j][p] * prods[j];
      ans ^= count.val();
    }
    ans %= MOD;
    cout << ans << '\n';
  }
}
