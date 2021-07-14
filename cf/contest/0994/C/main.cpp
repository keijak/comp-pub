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
const int L = 105;

typedef long double D;
typedef complex<D> P;
const D EPS = 1e-9;

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

bool isecLP(P a1, P a2, P b) {
  return abs(ccw(a1, a2, b)) != 1;  // return EQ(cross(a2-a1, b-a1), 0); と等価
}

P proj(P a1, P a2, P p) {
  return a1 + dot(a2 - a1, p - a1) / norm(a2 - a1) * (a2 - a1);
}

P reflection(P a1, P a2, P p) { return D(2.0) * proj(a1, a2, p) - p; }

D distLP(P a1, P a2, P p) { return abs(proj(a1, a2, p) - p); }

bool isecSS(P a1, P a2, P b1, P b2) {
  return ccw(a1, a2, b1) * ccw(a1, a2, b2) <= 0 &&
         ccw(b1, b2, a1) * ccw(b1, b2, a2) <= 0;
}

bool conta(vector<P> bq, vector<P> aq) {
  D bl = L * 5, br = -L * 5, bt = -L * 5, bb = L * 5;
  REP(j, 4) {
    chmin(bl, bq[j].real());
    chmax(br, bq[j].real());
    chmin(bb, bq[j].imag());
    chmax(bt, bq[j].imag());
  }
  REP(i, 4) {
    auto x = aq[i].real(), y = aq[i].imag();
    if (x + EPS < bl or x > br + EPS or y + EPS < bb or y > bt + EPS)
      return false;
  }
  return true;
}

bool solve() {
  D al = L, ar = -L, at = -L, ab = L;
  D bl = L, br = -L, bt = -L, bb = L;
  D bx = 0, by = 0;

  vector<P> ap, bp;
  vector<P> aq, bq;
  REP(i, 4) {
    D x, y;
    cin >> x >> y;
    ap.push_back(P(x, y));
    aq.push_back(P(x - y, x + y));
    chmin(al, x);
    chmax(ar, x);
    chmin(ab, y);
    chmax(at, y);
  }
  REP(i, 4) {
    D x, y;
    cin >> x >> y;
    bp.push_back(P(x, y));
    bq.push_back(P(x - y, x + y));
    chmin(bl, x);
    chmax(br, x);
    chmin(bb, y);
    chmax(bt, y);
    bx += x;
    by += y;
  }
  bx /= 4;
  by /= 4;

  // outside
  if (br + EPS < al) return false;
  if (bl > ar + EPS) return false;
  if (bb > at + EPS) return false;
  if (bt + EPS < ab) return false;

  if (conta(ap, bp)) return true;
  if (conta(bq, aq)) return true;

  REP(i, 4) {
    REP(j, 4) {
      if (isecSS(ap[i], ap[(i + 1) % 4], bp[j], bp[(j + 1) % 4])) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << (solve() ? "YES" : "NO") << "\n";
}
