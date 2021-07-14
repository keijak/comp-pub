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

using namespace std;

template <class T = i64>
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

void solve() {
  int n;
  cin >> n;
  vector<array<i64, 2>> points(n);
  REP(i, n) {
    i64 x, y;
    cin >> x >> y;
    points[i] = {x, y};
  }

  int m;
  cin >> m;
  vector<pair<int, i64>> ops(m);
  REP(i, m) {
    int op;
    cin >> op;
    if (op == 1 or op == 2) {
      ops[i] = {op, 0};
    } else {
      i64 p;
      cin >> p;
      ops[i] = {op, p};
    }
  }

  int q;
  cin >> q;
  vector<array<int, 2>> queries(q);
  map<int, vector<int>> mp;
  REP(i, q) {
    int a, b;
    cin >> a >> b;
    --b;
    queries[i] = {a, b};
    mp[a].push_back(i);
  }

  Matrix mat = Matrix<i64>::I(3);
  Matrix r1 = {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}};
  Matrix r2 = {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}};

  auto operate = [&](int i) -> void {
    auto [op, p] = ops[i];
    if (op == 1) {
      mat = r1 * mat;
    } else if (op == 2) {
      mat = r2 * mat;
    } else if (op == 3) {
      mat = Matrix<i64>{{-1, 0, 2 * p}, {0, 1, 0}, {0, 0, 1}} * mat;
    } else {
      assert(op == 4);
      mat = Matrix<i64>{{1, 0, 0}, {0, -1, 2 * p}, {0, 0, 1}} * mat;
    }
  };

  vector<array<i64, 2>> ans(q);
  REP(i, m + 1) {
    for (auto j : mp[i]) {
      auto [a, b] = queries[j];
      auto [x, y] = points[b];
      auto p = Matrix<i64>::column({x, y, 1});
      p = mat * p;
      ans[j] = {p[0][0], p[1][0]};
    }
    if (i == m) break;
    operate(i);
  }

  REP(i, q) {
    auto [x, y] = ans[i];
    cout << x << ' ' << y << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
