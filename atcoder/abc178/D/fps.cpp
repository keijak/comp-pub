#include <bits/stdc++.h>

#include <atcoder/convolution>

using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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

const int MOD = 1e9 + 7;

template <typename T, int asize>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^(asize - 1).
  std::array<T, asize> coeff;

  DenseFPS() : coeff{} {};  // zero-initialized
  DenseFPS(const DenseFPS &other) : coeff(other.coeff) {}
  DenseFPS(DenseFPS &&other) : coeff(std::move(other.coeff)) {}
  DenseFPS &operator=(const DenseFPS &other) { coeff = other.coeff; }
  DenseFPS &operator=(DenseFPS &&other) { coeff = std::move(other.coeff); }

  static int size() { return asize; }

  const T &operator[](int i) const { return coeff[i]; }

  DenseFPS &operator+=(const DenseFPS &other) {
    for (int i = 0; i < asize; ++i) {
      coeff[i] += other[i];
    }
    return *this;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    for (int i = 0; i < asize; ++i) {
      coeff[i] -= other[i];
    }
    return *this;
  }

  DenseFPS &operator*=(const DenseFPS &other) {
    std::array<T, asize> res = {};
    for (int i = 0; i < asize; ++i) {
      for (int j = 0; i + j < asize; ++j) {
        res[i + j] += (*this)[i] * other[j];
      }
    }
    coeff = std::move(res);
    return *this;
  }

  DenseFPS &mul_fft(const DenseFPS &other) {
    std::vector<long long> x(coeff.begin(), coeff.end());
    std::vector<long long> y(other.coeff.begin(), other.coeff.end());
    auto res = atcoder::convolution_ll(x, y);
    for (int i = 0; i < asize; ++i) {
      coeff[i] = res[i] % MOD;
    }
    return *this;
  }

  DenseFPS &mul_ntt(const DenseFPS &other) {
    std::vector<long long> x(coeff.begin(), coeff.end());
    std::vector<long long> y(other.coeff.begin(), other.coeff.end());
    auto res = atcoder::convolution<998244353>(x, y);
    for (int i = 0; i < asize; ++i) {
      coeff[i] = res[i];
    }
    return *this;
  }

  friend DenseFPS operator+(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res(x);
    res += y;
    return res;
  }

  friend DenseFPS operator-(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res(x);
    res -= y;
    return res;
  }

  friend DenseFPS operator*(const DenseFPS &x, const DenseFPS &y) {
    DenseFPS res(x);
    res *= y;
    return res;
  }

  DenseFPS pow(u64 t) const {
    DenseFPS base = *this;
    DenseFPS res(T(1));
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }
};

using mint = atcoder::modint1000000007;
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int S;
  cin >> S;

  DenseFPS<i64, 2005> f;
  for (int k = 3; k <= S; ++k) f.coeff[k] = 1;
  DenseFPS<i64, 2005> g = f;

  mint ans = 0;
  for (int i = 1; 3 * i <= S; ++i) {
    ans += g.coeff[S];
    g.mul_fft(f);
  }
  cout << ans.val() << endl;
}
