// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;
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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
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

template<class T>
struct Matrix {
  using Row = std::vector<T>;
  std::vector<Row> data;

  Matrix() {}
  Matrix(int n, int m) : data(n, Row(m, T(0))) {}
  explicit Matrix(int n) : data(n, Row(n, T(0))) {};
  explicit Matrix(std::vector<Row> a) : data(std::move(a)) {}
  Matrix(std::initializer_list<Row> a) : data(std::move(a)) {}

  inline int height() const { return (int) (data.size()); }

  inline int width() const { return (int) (data[0].size()); }

  inline const Row &operator[](int k) const { return data[k]; }

  inline Row &operator[](int k) { return data[k]; }

  static Matrix I(int n) {
    Matrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = T(1);
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
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        (*this)[i][j] += B[i][j];
      }
    }
    return *this;
  }

  Matrix &operator-=(const Matrix &B) {
    int n = height(), m = width();
    assert(n == B.height() and m == B.width());
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        (*this)[i][j] -= B[i][j];
      }
    }
    return *this;
  }

  Matrix &operator*=(const Matrix &B) {
    int n = height(), m = B.width(), p = width();
    assert(p == B.height());
    std::vector<Row> C(n, Row(m, T(0)));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        for (int k = 0; k < p; k++) {
          C[i][j] = (C[i][j] + (*this)[i][k] * B[k][j]);
        }
      }
    }
    std::swap(data, C);
    return *this;
  }

  Matrix &operator^=(long long k) {
    Matrix B = Matrix::I(height());
    while (k > 0) {
      if (k & 1) B *= *this;
      *this *= *this;
      k >>= 1LL;
    }
    data.swap(B.data);
    return *this;
  }

  Matrix operator+(const Matrix &B) const {
    Matrix r = *this;
    r += B;
    return r;
  }

  Matrix operator-(const Matrix &B) const {
    Matrix r = *this;
    r -= B;
    return r;
  }

  Matrix operator*(const Matrix &B) const {
    Matrix r = *this;
    r *= B;
    return r;
  }

  Matrix operator^(const long long k) const {
    Matrix r = *this;
    r ^= k;
    return r;
  }

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

auto solve() -> Mint {
  const Int N = in;
  const int pct_x = in, pct_y = in;
  const Mint px = Mint(pct_x) / 100, py = Mint(pct_y) / 100;
  const string C = in;
  assert(ssize(C) == 24);
  if (C == string(24, 'A')) {
    return 1;
  }
  if (C == string(24, 'T')) {
    return 0;
  }

  auto prob = [&](int i) -> Mint {
    return (C[i] == 'A') ? py : px;
  };
  Mint all_miss_p = 1;
  REP(i, 24) all_miss_p *= 1 - prob(i);
  Mint gsum = Mint(1) / (1 - all_miss_p);

  Mat T(24);
  REP(j, 24) {
    REP(k, 24) {
      Mint q = 1;
      if (j > k) {
        for (int t = k + 1; t < j; ++t) {
          q *= 1 - prob(t);
        }
      } else {
        for (int t = k + 1; t < 24 + j; ++t) {
          q *= 1 - prob(t % 24);
        }
      }
      q *= prob(j);
      q *= gsum;
      T[j][k] = q;
    }
  }

  auto f0 = Mat::column(vector<Mint>(24, 0));
  f0[23][0] = 1;
  auto fn = T.pow(N) * f0;
  Mint ans = 0;
  REP(i, 24) {
    DUMP(i, fn[i][0]);
    if (C[i] == 'A') ans += fn[i][0];
  }
  return ans;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
