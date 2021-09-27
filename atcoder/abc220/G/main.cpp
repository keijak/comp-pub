#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...)                                             \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b)
      os << sep;
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

template<typename T,
    typename = std::enable_if_t<is_iterable<T>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x)
        std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using Int = long long; //boost::multiprecision::int128_t;
using Rational = boost::rational<Int>;

struct P {
  i64 x, y, c;
};

template<class T>
T floor_div(T x, T y) {
  assert(y != 0);
  return x / y - (((x ^ y) < 0) and (x % y));
}

auto solve() {
  const int n = in;
  vector<P> points(n);
  for (auto &p: points) {
    p.x = i64(in) * 2;
    p.y = i64(in) * 2;
    p.c = i64(in);
  }

  map<array<Int, 4>, map<array<i64, 2>, i64>> mp;
  REP(i, n) {
    REP(j, i) {
      i64 dx = points[i].x - points[j].x;
      i64 dy = points[i].y - points[j].y;
      i64 mx = (points[i].x + points[j].x) / 2LL;
      i64 my = (points[i].y + points[j].y) / 2LL;

      pair<Int, Int> a0;
      Rational y0 = 0;
      if (dy == 0) {
        a0 = {0, 1};
        y0 = Rational(mx);
      } else if (dx == 0) {
        a0 = {1, 0};
        y0 = Rational(my);
      } else {
        Rational q(dx, dy);
        a0 = {q.numerator(), q.denominator()};
        y0 = my + q * mx;
      }
      array<Int, 4> key1 = {a0.first, a0.second, y0.numerator(), y0.denominator()};
      array<i64, 2> key2 = {mx, my};
      i64 val = points[i].c + points[j].c;
      chmax(mp[move(key1)][move(key2)], move(val));
    }
  }
  i64 ans = -1;
  for (const auto &[k, v]: mp) {
    if (ssize(v) < 2) {
      continue;
    }
    array<i64, 2> top2 = {};
    for (const auto&[k2, v2]: v) {
      if (chmax(top2[1], v2)) {
        if (top2[0] < top2[1]) {
          swap(top2[0], top2[1]);
        }
      }
    }
    chmax(ans, top2[0] + top2[1]);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
