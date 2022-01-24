#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = float;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct MinMaxRing {
  using T = int;
  static T add(const T &x, const T &y) { return std::min(x, y); }
  static T zero() { return std::numeric_limits<T>::max(); }
  static T mul(const T &x, const T &y) { return std::max(x, y); }
  static T one() { return 0; }
};

template<class Ring>
struct GenericMatrix {
  using T = typename Ring::T;
  using Row = std::vector<T>;
  std::vector<Row> data;

  GenericMatrix() = default;
  GenericMatrix(int n, int m) : data(n, Row(m, Ring::zero())) {}
  explicit GenericMatrix(int n) : data(n, Row(n, Ring::zero())) {};
  explicit GenericMatrix(std::vector<Row> a) : data(std::move(a)) {}
  GenericMatrix(std::initializer_list<Row> a) : data(std::move(a)) {}

  inline int height() const { return (int) (data.size()); }

  inline int width() const { return (int) (data[0].size()); }

  inline const Row &operator[](int k) const { return data[k]; }

  inline Row &operator[](int k) { return data[k]; }

  static GenericMatrix I(int n) {
    GenericMatrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = Ring::one();
    return mat;
  }

  static GenericMatrix row(std::vector<T> v) {
    GenericMatrix mat(1, v.size());
    mat[0] = std::move(v);
    return mat;
  }

  static GenericMatrix column(const std::vector<T> &v) {
    const int n = v.size();
    GenericMatrix mat(n, 1);
    for (int i = 0; i < n; ++i) mat[i][0] = v[i];
    return mat;
  }

  GenericMatrix transpose() const {
    int n = height(), m = width();
    GenericMatrix mat(m, n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        mat[j][i] = (*this)[i][j];
      }
    }
    return mat;
  }

  GenericMatrix &operator+=(const GenericMatrix &B) {
    int n = height(), m = width();
    assert(n == B.height() and m == B.width());
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        (*this)[i][j] = Ring::add((*this)[i][j], B[i][j]);
      }
    }
    return *this;
  }

  GenericMatrix &operator*=(const GenericMatrix &B) {
    int n = height(), m = B.width(), p = width();
    assert(p == B.height());
    std::vector<Row> C(n, Row(m, Ring::zero()));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        for (int k = 0; k < p; k++) {
          C[i][j] = Ring::add(C[i][j], Ring::mul((*this)[i][k], B[k][j]));
        }
      }
    }
    data.swap(C);
    return *this;
  }

  GenericMatrix &operator^=(long long k) {
    auto B = GenericMatrix::I(height());
    while (k > 0) {
      if (k & 1) B *= *this;
      *this *= *this;
      k >>= 1LL;
    }
    data.swap(B.data);
    return *this;
  }

  GenericMatrix operator+(const GenericMatrix &B) const { return (GenericMatrix(*this) += B); }

  GenericMatrix operator*(const GenericMatrix &B) const { return (GenericMatrix(*this) *= B); }

  GenericMatrix operator^(const long long k) const { return (GenericMatrix(*this) ^= k); }

  GenericMatrix pow(const long long k) const { return (*this) ^ k; }

  friend std::ostream &operator<<(std::ostream &os, const GenericMatrix &p) {
    int n = p.height();
    for (int i = 0; i < n; i++) {
      os << p[i] << "\n";
    }
    return os;
  }
};

auto solve() {
  int n = in, T = in, L = in;
  GenericMatrix<MinMaxRing> mat(n, n);
  REP(t, T) {
    int u = in, v = in;
    --u, --v;
    chmin(mat[v][u], t);
  }
  vector<int> vc(n, std::numeric_limits<int>::max());
  vc[0] = 0;
  auto c0 = GenericMatrix<MinMaxRing>::column(vc);
  auto ct = mat.pow(L) * c0;
  vector<int> ans(n, -1);
  REP(i, n) {
    ans[i] = (ct[i][0] == std::numeric_limits<int>::max()) ? -1 : ct[i][0] + 1;
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print_seq(ans);
  }
}
