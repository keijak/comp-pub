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
  return (int) a.size();
}

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
      for (auto &e: x) std::cin >> e;
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
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
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

using namespace std;
using Real = long double;

auto solve() {
  const string S = in, K = in;
  const int n = ssize(S);
  vector<char> known(256, false);
  for (char ch: K) known[ch] = true;
  vector<char> unknown(n, false);
  {
    set<char> seen;
    REP(i, n) {
      if (known[S[i]]) continue;
      if (seen.count(S[i])) continue;
      seen.insert(S[i]);
      unknown[i] = true;
    }
  }
  vector<int> cum(n + 1, 0);
  REP(i, n) {
    cum[i + 1] = cum[i] + unknown[i];
  }

  int unknown_chars = 0;
  for (char i = '0'; i <= '9'; ++i) {
    if (not known[i]) ++unknown_chars;
  }
  for (char i = 'a'; i <= 'z'; ++i) {
    if (not known[i]) ++unknown_chars;
  }

  auto dp = vector(n + 1, vector(unknown_chars + 1, Real(0)));
  REP(j, unknown_chars + 1) dp[n][j] = 0;

  for (int i = n - 1; i >= 0; --i) {
    if (not unknown[i]) {
      REP(j, unknown_chars + 1) {
        dp[i][j] = dp[i + 1][j] + 1;
      }
      continue;
    }
    assert(unknown_chars - cum[i] > 0);

    REP(j, unknown_chars - cum[i] + 1) {
      Real kp = Real(j) / (unknown_chars - cum[i]);
      Real expect1 = 0;
      if (j > 0) {
        expect1 += kp * (1 + dp[i + 1][j - 1]);
      }
      Real expect2 = 0;
      int u = unknown_chars - cum[i] - j;
      if (u > 0) {
        Real up = Real(1) / u;
        for (int q = 0; q < u; ++q) {
          expect2 += up * (1 + q * 2 + dp[i + 1][j + q]);
        }
        expect2 *= (1 - kp);
      }
      dp[i][j] = expect1 + expect2;
    }
  }
  return dp[0][0];
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
