#include <bits/stdc++.h>

#include <boost/rational.hpp>

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
using Rational = boost::rational<long long>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << fixed << setprecision(15);

  int n;
  cin >> n;
  vector<int> T(n), V(n);
  cin >> T >> V;

  const int VMAX = 100;
  auto dp = vector(n + 1, vector(VMAX + 1, (Rational)-100));
  dp[0][0] = 0;
  for (int i = 0; i < n; ++i) {
    int tlen = T[i], vlimit = V[i];
    REP(vs, vlimit + 1) {
      if (dp[i][vs] < Rational(0)) continue;
      Rational x0 = dp[i][vs];
      int vle = vlimit;
      if (i + 1 == n) {
        vle = 0;
      } else if (V[i + 1] < vlimit) {
        vle = V[i + 1];
      }
      for (int ve = max(vs - tlen, 0); ve <= min(vle, vs + tlen); ++ve) {
        int t1 = vlimit - vs;
        int t2 = tlen + ve - vlimit;
        if (t1 <= t2) {
          //  八
          Rational x1 = Rational(1, 2) * t1 * t1 + vs * t1;
          Rational x2 = vlimit * (t2 - t1);
          Rational x3 = (-Rational(1, 2) * tlen * tlen + (tlen + ve) * tlen) -
                        (-Rational(1, 2) * t2 * t2 + (tlen + ve) * t2);
          chmax(dp[i + 1][ve], x0 + x1 + x2 + x3);
        } else {
          // x
          Rational tm = tlen + ve - vs;
          tm /= 2;
          assert(0 <= tm and tm <= tlen);
          Rational x1 = Rational(1, 2) * tm * tm + vs * tm;
          Rational x3 = (-Rational(1, 2) * tlen * tlen + (tlen + ve) * tlen) -
                        (-Rational(1, 2) * tm * tm + (tlen + ve) * tm);
          chmax(dp[i + 1][ve], x0 + x1 + x3);
        }
      }
    }
  }

  Rational ans = 0;
  REP(v, VMAX + 1) { chmax(ans, dp[n][v]); }
  cout << boost::rational_cast<double>(ans) << endl;
}
