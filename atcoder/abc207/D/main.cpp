#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
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
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__);

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
using P = complex<Real>;
Real EPS = 1e-6;

auto solve() -> bool {
  INPUT(int, n);
  vector<P> S(n), T(n);

  P g1(0.0, 0.0), g2(0.0, 0.0);
  REP(i, n) {
    INPUT(int, x, y);
    P p(x, y);
    S[i] = p;
    g1 += p;
  }
  g1 /= Real(n);
  REP(i, n) {
    INPUT(int, x, y);
    P p(x, y);
    T[i] = p;
    g2 += p;
  }
  g2 /= Real(n);
  REP(i, n) {
    S[i] -= g1;
    T[i] -= g2;
  }
  DUMP(g1, S);
  DUMP(g2, T);
  int key = -1;
  bool t_all_center = true;
  REP(i, n) {
    if (abs(S[i]) > EPS) {
      key = i;
    }
    if (abs(T[i]) > EPS) {
      t_all_center = false;
    }
  }
  if (key == -1) {
    return t_all_center;
  }
  Real kr = abs(S[key]);
  Real ka = arg(S[key]);
  const Real B = 1e6;
  REP(i, n) {
    if (abs((Real) abs(T[i]) - kr) > EPS) continue;
    P z = polar(Real(1.0), arg(T[i]) - ka);

    assert(abs(T[i] - (S[key] * z)) < EPS);

    multiset<pair<i64, i64>> ts;
    for (const auto &x : T) {
      i64 xx = llround(x.real() * B);
      i64 yy = llround(x.imag() * B);
      ts.insert({xx, yy});
    }
    bool ok = true;
    for (const auto &x : S) {
      auto p = x * z;
      i64 xx = llround(p.real() * B);
      i64 yy = llround(p.imag() * B);
      auto it = ts.find({xx, yy});
      if (it == ts.end()) {
        ok = false;
        break;
      }
      ts.erase(it);
    }
    if (ok) {
      return true;
    }
    DUMP(ts);
  }
  return false;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "Yes" : "No");
  }
}
