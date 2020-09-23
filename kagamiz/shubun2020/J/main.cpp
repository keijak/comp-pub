

#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
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
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
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

i64 factorial(int x) {
  static vector<i64> facts = {1, 1, 2};
  facts.reserve(x + 1);
  while (SIZE(facts) <= x) {
    facts.push_back(facts.back() * SIZE(facts));
  }
  return facts[x];
}

void solve1() {
  int n;
  cin >> n;
  V<int> y(n);
  cin >> y;

  V<bool> used(n);
  i64 ans = 0;
  REP(i, n) {
    i64 cnt = 0;
    for (int j = 0; j < y[i]; ++j) {
      if (not used[j]) ++cnt;
    }
    used[y[i]] = true;
    ans += cnt * factorial(n - 1 - i);
  }
  cout << ans << '\n';
}

V<int> read_vec(int n) {
  V<int> res(n);
  string buf;
  REP(i, n) {
    cin >> buf;
    if (buf[0] == '_') {
      buf[0] = '-';
    }
    res[i] = stoi(buf);
  }
  return res;
}

void solve2() {
  int n;
  cin >> n;
  auto y = read_vec(n);
  if (n == 1) {
    cout << y[0] << '\n';
    return;
  }
  i64 ans = y[n - 2] - y[n - 1];
  for (int i = n - 3; i >= 0; --i) {
    ans = y[i] - ans;
  }
  cout << ans << '\n';
}

struct crc32 {
  uint32_t table[256];

  void generate_table(uint32_t polynomial) {
    for (uint32_t i = 0; i < 256; i++) {
      uint32_t c = i;
      for (size_t j = 0; j < 8; j++) {
        if (c & 1) {
          c = polynomial ^ (c >> 1);
        } else {
          c >>= 1;
        }
      }
      table[i] = c;
    }
  }

  uint32_t update(uint32_t initial, const string &buf) {
    uint32_t c = initial;
    for (size_t i = 0; i < buf.size(); ++i) {
      c = table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFF;
  }
};

void solve3() {
  auto x = read_vec(2);
  string y;
  cin >> y;
  crc32 c;
  c.generate_table(x[0]);
  int32_t crc = c.update(x[1], y);
  cout << crc << '\n';
}

void solve4() {
  auto x = read_vec(3);
  int n;
  cin >> n;
  u64 t_size = x[0] * (u64)x[1] * x[2];
  u64 m = 1;
  auto y = read_vec(n);
  for (i64 z : y) {
    if (m < t_size) m *= z;
  }
  chmin(m, t_size);

  auto t = make_vec(x[0], x[1], x[2], 0LL);
  int counter = 0;
  REP(i, x[0]) REP(j, x[1]) REP(k, x[2]) {
    t[i][j][k] = counter;
    counter = (counter + 1) % m;
  }
  DEBUG(x[0], x[1], x[2]);
  REP(j, x[1]) {
    REP(k, x[2]) {
      u64 val = 0;
      REP(i, x[0]) { val += t[i][j][k]; }
      if (k) cout << " ";
      cout << val;
    }
    cout << endl;
  }
}

template <class T>
struct Matrix {
  using Row = vector<T>;
  vector<Row> A;

  Matrix() {}

  Matrix(int n, int m) : A(n, Row(m, 0)) {}

  Matrix(int n) : A(n, Row(n, 0)){};

  int height() const { return (int)(A.size()); }

  int width() const { return (int)(A[0].size()); }

  inline const Row &operator[](int k) const { return (A.at(k)); }

  inline Row &operator[](int k) { return (A.at(k)); }

  static Matrix I(int n) {
    Matrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = 1;
    return (mat);
  }

  Matrix &operator+=(const Matrix &B) {
    size_t n = height(), m = width();
    assert(n == B.height() && m == B.width());
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) (*this)[i][j] += B[i][j];
    return (*this);
  }

  Matrix &operator-=(const Matrix &B) {
    size_t n = height(), m = width();
    assert(n == B.height() && m == B.width());
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) (*this)[i][j] -= B[i][j];
    return (*this);
  }

  Matrix &operator*=(const Matrix &B) {
    int n = height(), m = B.width(), p = width();
    assert(p == B.height());
    vector<Row> C(n, Row(m, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        for (int k = 0; k < p; k++)
          C[i][j] = C[i][j] + ((*this)[i][k] * B[k][j]);
    A.swap(C);
    return (*this);
  }
  Matrix &mulbool(const Matrix &B) {
    int n = height(), m = B.width(), p = width();
    assert(p == B.height());
    vector<Row> C(n, Row(m, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        for (int k = 0; k < p; k++)
          C[i][j] = C[i][j] or ((*this)[i][k] and B[k][j]);
    A.swap(C);
    return (*this);
  }

  Matrix operator+(const Matrix &B) const { return (Matrix(*this) += B); }

  Matrix operator-(const Matrix &B) const { return (Matrix(*this) -= B); }

  Matrix operator*(const Matrix &B) const { return (Matrix(*this) *= B); }

  Matrix operator^(const long long k) const { return (Matrix(*this) ^= k); }

  friend ostream &operator<<(ostream &os, Matrix &p) {
    size_t n = p.height(), m = p.width();
    for (int i = 0; i < n; i++) {
      os << "[";
      for (int j = 0; j < m; j++) {
        os << p[i][j] << (j + 1 == m ? "]\n" : ",");
      }
    }
    return (os);
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
      if (idx == -1) return (0);
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
    return (ret);
  }
};

void solve5() {
  int n;
  cin >> n;
  auto x = read_vec(n);
  int m;
  cin >> m;
  auto y = read_vec(n);
  assert(m == n);
  Matrix<int> a(n), b(n, 1);
  REP(i, n) {
    if (x[i] < n) {
      a[x[i]][i] = 1;
    }
    b[i][0] = y[i];
  }
  REP(i, n) { a[i][i] = 1; }

  int k = 0;
  while ((1LL << k) < n) {
    ++k;
  }
  REP(i, k) { a.mulbool(a); }
  auto ans = a * b;
  REP(i, n) {
    if (i) cout << ' ';
    cout << ans[i][0];
  }
  cout << endl;
}

void hoge() { cout << "hoge" << endl; }

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  V<function<void(void)>> solvers = {solve1, solve2, solve3, solve4,
                                     solve5, hoge,   hoge,   hoge};
  int t;
  cin >> t;
  solvers[t - 1]();
}
