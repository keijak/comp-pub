#include <bits/stdc++.h>

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template <class T = Mint>
struct Matrix {
  using Row = std::vector<T>;
  std::vector<Row> data;

  Matrix() {}
  Matrix(int n, int m) : data(n, Row(m, 0)) {}
  explicit Matrix(int n) : data(n, Row(n, 0)){};
  explicit Matrix(std::vector<Row> a) : data(std::move(a)) {}
  Matrix(std::initializer_list<Row> a) : data(std::move(a)) {}

  inline int height() const { return (int)(data.size()); }

  inline int width() const { return (int)(data[0].size()); }

  inline const Row &operator[](int k) const { return data[k]; }

  inline Row &operator[](int k) { return data[k]; }

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

void solve() {
  i64 ma, na, mb, nb, S, T, K;
  cin >> ma >> na >> S;
  cin >> mb >> nb >> T;
  cin >> K;

  Mint pa = Mint(ma) / na;
  Mint pb = Mint(mb) / nb;

  const int N = S + T + 1;
  Matrix<Mint> A(N), B(N), C(N);
  A[0][0] = 1;
  A[N - 1][N - 1] = 1;
  for (int i = 1; i < N - 1; ++i) {
    Mint ps = 1;
    REP(j, i) {
      A[i - j][i] = (1 - pa) * ps;
      ps *= pa;
    }
    A[0][i] = ps;
  }

  B[0][0] = 1;
  B[N - 1][N - 1] = 1;
  for (int i = 1; i < N - 1; ++i) {
    Mint ps = 1;
    for (int j = i; j < N - 1; ++j) {
      B[j][i] = (1 - pb) * ps;
      ps *= pb;
    }
    B[N - 1][i] = ps;
  }

  Matrix<Mint> v = Matrix<Mint>::column(vector<Mint>(N, 0));
  v[S][0] = 1;

  v = (B * A).pow(K) * v;
  Mint alpha = v[0][0];
  Mint beta = v[N - 1][0];
  cout << alpha << endl;
  cout << beta << endl;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}