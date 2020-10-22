#include <bits/stdc++.h>

#include <atcoder/convolution>
#include <atcoder/modint>

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

// Formal Power Series.
template <typename T, int MAX_DEGREE>
struct DenseFPS {
  // Coefficients of terms from x^0 to x^MAX_DEGREE.
  std::vector<T> coeff;

  DenseFPS() : coeff(MAX_DEGREE + 1) {}  // zero-initialized
  explicit DenseFPS(std::vector<T> c) : coeff(std::move(c)) {}

  DenseFPS(const DenseFPS &other) : coeff(other.coeff) {}
  DenseFPS(DenseFPS &&other) : coeff(std::move(other.coeff)) {}
  DenseFPS &operator=(const DenseFPS &other) {
    coeff = other.coeff;
    return *this;
  }
  DenseFPS &operator=(DenseFPS &&other) {
    coeff = std::move(other.coeff);
    return *this;
  }

  static constexpr int size() { return MAX_DEGREE + 1; }

  const T &operator[](int i) const { return coeff[i]; }

  DenseFPS &operator+=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) {
      coeff[i] += other[i];
    }
    return *this;
  }

  DenseFPS &operator-=(const DenseFPS &other) {
    for (int i = 0; i < size(); ++i) {
      coeff[i] -= other[i];
    }
    return *this;
  }

  DenseFPS &operator*=(const DenseFPS &other) {
    std::vector<T> res(size());
    for (int i = 0; i < size(); ++i) {
      for (int j = 0; i + j < size(); ++j) {
        res[i + j] += (*this)[i] * other[j];
      }
    }
    coeff = std::move(res);
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

// Fast polynomial multiplication by single NTT.
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> mul_ntt(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                     const DenseFPS<ModInt, MAX_DEGREE> &y) {
  static_assert(ModInt::mod() != 1'000'000'007);  // Must be NTT-friendly MOD!
  auto z = atcoder::convolution(x.coeff, y.coeff);
  z.resize(MAX_DEGREE + 1);  // Maybe shrink.
  return {std::move(z)};
}

// Polynomial multiplication by NTT + Garner (arbitrary mod).
template <typename ModInt, int MAX_DEGREE>
DenseFPS<ModInt, MAX_DEGREE> mul_mod(const DenseFPS<ModInt, MAX_DEGREE> &x,
                                     const DenseFPS<ModInt, MAX_DEGREE> &y) {
  std::vector<i64> xll(x.size()), yll(y.size());
  for (int i = 0; i < x.size(); ++i) {
    xll[i] = x[i].val();
  }
  for (int i = 0; i < y.size(); ++i) {
    yll[i] = y[i].val();
  }
  auto zll = atcoder::convolution_ll(xll, yll);
  DenseFPS<ModInt, MAX_DEGREE> res;
  for (int i = 0; i <= MAX_DEGREE; ++i) {
    res.coeff[i] = zll[i];
  }
  return res;
}

// Polynomial multiplication by NTT + Garner (long long).
template <int MAX_DEGREE>
DenseFPS<i64, MAX_DEGREE> mul_ll(const DenseFPS<i64, MAX_DEGREE> &x,
                                 const DenseFPS<i64, MAX_DEGREE> &y) {
  auto z = atcoder::convolution_ll(x.coeff, y.coeff);
  z.resize(MAX_DEGREE + 1);  // Maybe shrink.
  return DenseFPS<i64, MAX_DEGREE>(std::move(z));
}

using namespace std;
using mint = atcoder::modint1000000007;
const int MOD = 1e9 + 7;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int S;
  cin >> S;

  DenseFPS<mint, 2000> f;
  for (int k = 3; k <= S; ++k) f.coeff[k] = 1;
  auto g = f;

  mint ans = 0;
  for (int i = 1; 3 * i <= S; ++i) {
    ans += g.coeff[S];
    g = mul_mod(g, f);
  }
  cout << ans.val() << endl;
}
