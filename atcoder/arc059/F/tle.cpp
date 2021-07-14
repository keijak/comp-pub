#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}
using namespace std;

template <class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n > 0 and n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination (nCk)
  T C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }

  // Permutation (nPk)
  T P(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[n - k];
  }
};

template <class T>
struct Matrix {
  using Row = std::vector<T>;
  std::vector<Row> A;

  Matrix() {}
  Matrix(int n, int m) : A(n, Row(m, 0)) {}
  explicit Matrix(int n) : A(n, Row(n, 0)){};
  explicit Matrix(std::vector<Row> a) : A(std::move(a)) {}
  Matrix(std::initializer_list<Row> a) : A(std::move(a)) {}

  inline int height() const { return (int)(A.size()); }

  inline int width() const { return (int)(A[0].size()); }

  inline const Row &operator[](int k) const { return A[k]; }

  inline Row &operator[](int k) { return A[k]; }

  static Matrix I(int n) {
    Matrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = 1;
    return mat;
  }

  static Matrix row(std::vector<T> v) {
    Matrix mat(1, v.size());
    mat[0] = std::move(v);
    return mat;
  }

  static Matrix column(const std::vector<T> &v) {
    const int n = v.size();
    Matrix mat(n, 1);
    for (int i = 0; i < n; ++i) mat[i][0] = v[i];
    return mat;
  }

  Matrix transpose() const {
    int n = height(), m = width();
    Matrix mat(m, n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        mat[j][i] = (*this)[i][j];
      }
    }
    return mat;
  }

  Matrix &operator+=(const Matrix &B) {
    int n = height(), m = width();
    assert(n == B.height() and m == B.width());
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j) (*this)[i][j] += B[i][j];
    return *this;
  }

  Matrix &operator-=(const Matrix &B) {
    int n = height(), m = width();
    assert(n == B.height() and m == B.width());
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j) (*this)[i][j] -= B[i][j];
    return *this;
  }

  Matrix &operator*=(const Matrix &B) {
    int n = height(), m = B.width(), p = width();
    assert(p == B.height());
    std::vector<Row> C(n, Row(m, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        for (int k = 0; k < p; k++)
          C[i][j] = (C[i][j] + (*this)[i][k] * B[k][j]);
    std::swap(A, C);
    return *this;
  }

  Matrix &operator^=(long long k) {
    Matrix B = Matrix::I(height());
    while (k > 0) {
      if (k & 1) B *= *this;
      *this *= *this;
      k >>= 1LL;
    }
    A.swap(B.A);
    return *this;
  }

  Matrix operator+(const Matrix &B) const { return (Matrix(*this) += B); }

  Matrix operator-(const Matrix &B) const { return (Matrix(*this) -= B); }

  Matrix operator*(const Matrix &B) const { return (Matrix(*this) *= B); }

  Matrix operator^(const long long k) const { return (Matrix(*this) ^= k); }

  Matrix pow(const long long k) const { return (*this) ^ k; }

  friend std::ostream &operator<<(std::ostream &os, const Matrix &p) {
    int n = p.height();
    for (int i = 0; i < n; i++) {
      os << p[i] << "\n";
    }
    return os;
  }

  T det() {
    Matrix B(*this);
    assert(width() == height());
    T ret = 1;
    for (int i = 0; i < width(); i++) {
      int idx = -1;
      for (int j = i; j < width(); j++) {
        if (B[j][i] != 0) idx = j;
      }
      if (idx == -1) return 0;
      if (i != idx) {
        ret *= -1;
        swap(B[i], B[idx]);
      }
      ret *= B[i][i];
      T vv = B[i][i];
      for (int j = 0; j < width(); j++) {
        B[i][j] /= vv;
      }
      for (int j = i + 1; j < width(); j++) {
        T a = B[j][i];
        for (int k = 0; k < width(); k++) {
          B[j][k] -= B[i][k] * a;
        }
      }
    }
    return ret;
  }
};
using Mat = Matrix<Mint>;

i64 solve() {
  int n;
  string s;
  cin >> n >> s;
  int ssz = ssize(s);
  int redundancy = n - ssz;
  Factorials fs(n * 2 + 5);

  auto catalan_number = [&](int k) -> Mint {
    return fs.C(2 * k, k) - fs.C(2 * k, k - 1);
  };

  DEBUG(n, ssz, redundancy);
  REP(i, 10) {
    if (i >= n * 2) break;
    DEBUG(i, catalan_number(i));
  }

  // init
  // dp1[i][j] := typed i times, j chars remaining
  vector dp1(redundancy + 1, vector(redundancy + 1, Mint(0)));
  dp1[0][0] = 1;
  REP(i, redundancy) {
    REP(j, redundancy + 1) {
      if (j + 1 <= redundancy) dp1[i + 1][j + 1] += dp1[i][j] * 2;
      if (j > 0) {
        dp1[i + 1][j - 1] += dp1[i][j];
      } else if (j == 0) {
        dp1[i + 1][j] += dp1[i][j];
      }
    }
  }
  if (n == 3) {
    REP(i, redundancy + 1) { DEBUG(i, dp1[i]); }
  }

  const int m = redundancy / 2;
  vector<Mint> v0(m + 1);
  REP(i, redundancy + 1) {
    int left = redundancy - i;
    if (left & 1) continue;
    v0[left / 2] = dp1[i][0];
  }
  if (n == 3) {
    DEBUG(v0);
  }

  vector<Mint> pow2(m + 1);
  pow2[0] = 1;
  REP(i, m) { pow2[i + 1] = pow2[i] * 2; }

  Mat A(m + 1);
  REP(i, m + 1) {
    for (int j = i; j <= m; ++j) {
      A[i][j] = catalan_number(j - i) * pow2[j - i];
    }
  }

  // cerr << "power start" << endl;
  auto va = Mat::column(v0);
  if (ssz <= 100) {
    REP(i, ssz) { va = A * va; }
  } else {
    A ^= ssz;
    va = A * va;
  }
  //  A ^= ssz;  // TLE: Θ(m^3 log(ssz))
  // cerr << "power end" << endl;

  // auto va = A * Mat::column(v0);
  return va[0][0].val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
