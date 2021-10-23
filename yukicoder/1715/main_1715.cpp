#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
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
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      static_assert(std::numeric_limits<T>::max() != T());  // max must be defined
      return T(sign) * (std::numeric_limits<T>::max() / T(4));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

// 2 largest items in an interval.
struct Max2Op {
  using value_type = pair<Int, int>;
  using T = std::array<value_type, 2>;

  static T op(const T &x, const T &y) {
    std::array<value_type, 4> a = {x[0], x[1], y[0], y[1]};
    std::sort(std::rbegin(a), std::rend(a));
    return {a[0], a[1]};
  }
  static constexpr T id() {
    return {pair{(Int) -kBig, -1},
            pair{(Int) -kBig, -1}};
  }
};

template<class T>
T floor_div(T x, T y) {
  check(y != 0);
  return x / y - (((x ^ y) < 0) and (x % y));
}

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::abs(true_x - false_x) > 1) {
    T mid = floor_div<T>(true_x + false_x, 2);
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

auto solve() -> Int {
  int n = in, D = in;
  vector<pair<Int, Int>> menu(n);
  Int pmin = 0;
  REP(i, n) {
    Int p = in, q = in;
    menu[i].first = p;
    menu[i].second = q;
    chmin(pmin, -p);
  }

  auto two_sort = [&](vector<pair<Int, int>> &v) {
    if (ssize(v) < 2) return;
    if (v[0].first < v[1].first) swap(v[0], v[1]);
  };

  Int ans = bisect<Int>((Int) pmin * D, 0, [&](Int x) -> bool {
    auto dp = vector(2, vector(n, (Int) -kBig));
    REP(i, n) {
      auto[pi, qi] = menu[i];
      if (-pi < x) continue;
      dp[0][i] = -pi + qi;
    }
    REP(day, 1, D) {
      vector<pair<Int, int>> temp;
      REP(j, n) {
        auto z = dp[0][j];
        if (z < x) continue;
        if (ssize(temp) < 2) {
          temp.emplace_back(z, j);
          two_sort(temp);
        } else if (temp.back().first < z) {
          temp[1].first = z;
          temp[1].second = j;
          two_sort(temp);
        }
      }
      if (temp.empty()) return false;
      REP(j, n) {
        auto[pj, qj] = menu[j];
        if (j != temp[0].second) {
          auto z = temp[0].first;
          if (z - pj >= x) dp[1][j] = z - pj + qj;
        } else if (ssize(temp) >= 2) {
          auto z = temp[1].first;
          if (z - pj >= x) dp[1][j] = z - pj + qj;
        }
      }
      dp[0].swap(dp[1]);
      dp[1].assign(n, (Int) -kBig);
    }
    REP(j, n) {
      if (dp[0][j] >= x) return true;
    }
    return false;
  });
  check(ans != -kBig);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
