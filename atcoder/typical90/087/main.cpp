#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
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
  read_from_cin(__VA_ARGS__)

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto opt_x = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class F>
i64 bisect(i64 true_x, i64 false_x, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, i64>, "F must be i64->bool");
  // To allow negative values, use floor_div.
  assert(true_x >= 0 and false_x >= 0);

  while (std::abs(true_x - false_x) > 1) {
    auto mid = (true_x + false_x) / 2;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

auto solve() -> optional<i64> {
  INPUT(i64, n, P, K);
  auto A = vector(n, vector(n, 0LL));
  REP(i, n) {
    REP(j, n) {
      cin >> A[i][j];
    }
  }

  auto calc = [&](i64 X) -> int {
    auto D = vector(n, vector(n, P + 1));
    REP(i, n) D[i][i] = 0;
    REP(i, n) REP(j, n) {
        if (A[i][j] >= 0) {
          D[i][j] = A[i][j];
        } else {
          D[i][j] = min(X, P + 1);
        }
      }
    REP(k, n) REP(i, n) REP(j, n) {
          chmin(D[i][j], D[i][k] + D[k][j]);
        }
    int count = 0;
    REP(i, n) REP(j, i + 1, n) {
        if (D[i][j] <= P) ++count;
      }
    return count;
  };
  int v1 = calc(1);
  if (v1 < K) return 0;
  int vinf = calc(P + 1);
  if (vinf > K) return 0;
  if (vinf == K) return nullopt;
  i64 r = bisect(1LL, P + 1, [&](i64 x) {
    return calc(x) >= K;
  });
  i64 l = bisect(P + 1, 0LL, [&](i64 x) {
    return calc(x) <= K;
  });
  DUMP(l, r);
  if (r - l < 0) return 0LL;
  return r - l + 1;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print("Infinity");
    } else {
      print(ans.value());
    }
  }
}
