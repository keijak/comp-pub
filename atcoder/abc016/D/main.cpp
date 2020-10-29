#include <bits/stdc++.h>
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

const double EPS = 1e-6;
const double PI = std::cos(-1);
using D = double;
using P = std::complex<double>;

D dot(P a, P b) { return (conj(a) * b).real(); }
D cross(P a, P b) { return (conj(a) * b).imag(); }

int ccw(P a, P b, P c) {
  b -= a;
  c -= a;
  if (cross(b, c) > EPS) return +1;   // counter clockwise
  if (cross(b, c) < -EPS) return -1;  // clockwise
  if (dot(b, c) < -EPS) return +2;    // c--a--b on line
  if (norm(b) < norm(c)) return -2;   // a--b--c on line or a==b
  return 0;                           // a--c--b on line or a==c or b==c
}

bool isecSS(P a1, P a2, P b1, P b2) {
  return ccw(a1, a2, b1) * ccw(a1, a2, b2) <= 0 &&
         ccw(b1, b2, a1) * ccw(b1, b2, a2) <= 0;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  double ax, ay, bx, by, n;
  cin >> ax >> ay >> bx >> by >> n;
  P pa = {ax, ay}, pb = {bx, by};

  double tx, ty;
  cin >> tx >> ty;
  P pt = {tx, ty};
  P p0 = pt;
  int count = 0;
  REP(i, n) {
    P ps;
    if (i == n - 1) {
      ps = p0;
    } else {
      double sx, sy;
      cin >> sx >> sy;
      ps = {sx, sy};
    }
    if (isecSS(pa, pb, pt, ps)) {
      ++count;
    }
    pt = ps;
  }
  assert(count % 2 == 0);
  cout << (count / 2 + 1) << endl;
}
