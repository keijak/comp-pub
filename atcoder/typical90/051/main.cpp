#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::int128_t;
// using Int = boost::multiprecision::cpp_int;

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

auto solve() {
  int N, K;
  i64 P;
  cin >> N >> K >> P;
  const int q = N / 2;
  const int r = N - q;
  vector<Int> B(q), C(r);
  REP(i, N) {
    i64 x;
    cin >> x;
    if (i < q) {
      B[i] = Int(x);
    } else {
      C[i - q] = Int(x);
    }
  }
  DUMP(B);
  DUMP(C);

  vector<vector<Int>> bc(K + 1), cc(K + 1);
  REP(bits, 1 << q) {
    i64 c = 0;
    Int s = 0;
    REP(i, q) {
      if (bits >> i & 1) s += B[i], ++c;
    }
    if (c <= K and s <= P) bc[c].push_back(s);
  }
  REP(bits, 1 << r) {
    i64 c = 0;
    Int s = 0;
    REP(i, r) {
      if (bits >> i & 1) s += C[i], ++c;
    }
    if (c <= K and s <= P) cc[c].push_back(s);
  }
  REP(i, K + 1) {
    sort(ALL(bc[i]));
    sort(ALL(cc[i]));
  }
  i64 ans = 0;
  for (int ci = 0, bi = K; ci <= K; ++ci, --bi) {
    const auto &bv = bc[bi];
    const auto &cv = cc[ci];
    const int bn = ssize(bv);
    const int cn = ssize(cv);
    int bj = 0;
    for (int cj = cn - 1; cj >= 0; --cj) {
      while (bj < bn and bv[bj] + cv[cj] <= P) ++bj;
      ans += bj;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
