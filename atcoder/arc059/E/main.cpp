#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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
using Mint = atcoder::modint1000000007;

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

  DenseFPS &operator/=(const T &scalar) {
    for (auto &x : coeff_) x /= scalar;
    return *this;
  }
  friend DenseFPS operator/(const DenseFPS &x, const T &scalar) {
    DenseFPS res = x;
    res /= scalar;
    return res;
  }
  DenseFPS &operator/=(const DenseFPS &other) {
    return *this *= other.inv_naive();
  }
  friend DenseFPS operator/(const DenseFPS &x, const DenseFPS &y) {
    return x * y.inv_naive();
  }

 private:
  // Naive multiplication. O(N^2)
  DenseFPS mul_naive(const DenseFPS &other) const {
    const int n = std::min(size() + other.size() - 1, DMAX + 1);
    std::vector<T> res(n);
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; j < other.size(); ++j) {
        if (i + j >= n) break;
        res[i + j] += coeff_[i] * other.coeff_[j];
      }
    }
    return DenseFPS(std::move(res));
  }

  // Naive inverse. O(N^2)
  DenseFPS inv_naive() const {
    std::vector<T> res(size());
    res[0] = coeff_[0].inv();
    for (int i = 1; i < size(); ++i) {
      T s = 0;
      for (int j = 1; j <= i; ++j) {
        s += coeff_[j] * res[i - j];
      }
      res[i] = -res[0] * s;
    }
    return DenseFPS(std::move(res));
  }
};

const int CMAX = 400;
using FPS = DenseFPS<Mint, CMAX>;

Mint solve() {
  int N, C;
  cin >> N >> C;
  V<int> A(N), B(N);
  cin >> A >> B;

  FPS f({1});
  REP(i, N) {
    V<Mint> v(C + 1);
    for (int x = A[i]; x <= B[i]; ++x) {
      Mint a = 1;
      REP(j, C + 1) {
        v[j] += a;
        a *= x;
      }
    }
    f *= FPS(v);
  }
  return f[C];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve().val() << endl;
}
