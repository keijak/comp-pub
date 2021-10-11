#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")
#pragma GCC target("avx2")

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

struct VersatileInput {
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

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
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

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

auto solve() {
  const int n = in;
  vector<string> ss(n);
  vector<int> delta(n), minh(n), minfreq(n);
  vector<vector<int>> f(n);
  REP(i, n) {
    string s = in;
    ss[i] = s;
    f[i].resize(ssize(s) + 1);
    int h = 0, mh = kBig, mf = 0;
    REP(j, ssize(s)) {
      if (s[j] == '(') ++h; else --h;
      if (h == mh) {
        ++mf;
      } else if (h < mh) {
        mh = h;
        mf = 1;
      }
      if (h <= 0 and h == mh) {
        ++f[i][-h];
      }
    }
    delta[i] = h;
    minh[i] = mh;
    minfreq[i] = mf;
  }

  const unsigned kFull = (1 << n) - 1;
  auto dp = vector(1 << n, vector(2, (int) -kBig));
  dp[0][0] = 0;
  REP(bits, 1, 1 << n) {
    int h_all = 0;
    REP(i, n) {
      if (has_bit(bits, i)) {
        h_all += delta[i];
      }
    }
    REP(i, n) {
      if (not has_bit(bits, i)) continue;
      unsigned pbits = bits & ~(1 << i);
      if (dp[pbits][1] != -kBig) {
        chmax(dp[bits][1], dp[pbits][1]);
      }
      if (dp[pbits][0] == -kBig) continue;

      int h0 = h_all - delta[i];
      if (h0 < 0) continue;

      if (h0 + minh[i] > 0) {
        chmax(dp[bits][0], dp[pbits][0]);
      } else if (h0 + minh[i] == 0) {
        chmax(dp[bits][0], dp[pbits][0] + minfreq[i]);
      } else if (h0 < ssize(f[i])) {
        chmax(dp[bits][1], dp[pbits][0] + f[i][h0]);
      } else {
        chmax(dp[bits][1], dp[pbits][0]);
      }
    }
  }
  return max(dp[kFull][0], dp[kFull][1]);
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
