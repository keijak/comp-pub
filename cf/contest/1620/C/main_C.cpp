#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
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
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

std::vector<std::pair<char, int>> runlength(const string &a) {
  std::vector<std::pair<char, int>> res;
  const int n = a.size();
  if (n == 0) return res;
  res.reserve(n);
  res.emplace_back(a[0], 1);
  for (int i = 1; i < n; ++i) {
    if (a[i] == res.back().first) {
      res.back().second++;
    } else {
      res.emplace_back(a[i], 1);
    }
  }
  return res;
}

Int kMax = std::numeric_limits<Int>::max();

template<class T>
T saturating_mul(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  if (T res; not __builtin_mul_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return ((x ^ y) < 0) ? kMin : kMax;
  }
}

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

template<typename T>
T power(T b, Int e) {
  assert(e >= 0);
  T x = 1;
  while (e > 0) {
    if (e & 1) x = saturating_mul<Int>(x, b);
    b = saturating_mul<Int>(b, b);
    b *= b;
    e >>= 1;
  }
  return x;
}

template<typename T>
std::vector<T> pow_seq(int n, int base = 2) {
  assert(n >= 0 and base > 0);
  std::vector<T> p(n + 1);
  p[0] = 1;
  for (int i = 0; i < n; ++i) {
    p[i + 1] = saturating_mul<Int>(p[i], base);
  }
  return p;
}

auto solve() {
  Int n = in, K = in, X = in;
  string S = in;
  assert(ssize(S) == n);
  auto T = runlength(S);
  DUMP(T);
  int m = ssize(T);
  auto rest = vector(m + 1, 0LL);
  Int last = 1;
  for (int i = m - 1; i >= 0; --i) {
    if (T[i].first == '*') {
      rest[i] = last;
      Int kx = T[i].second * K;
      last = saturating_mul<Int>(last, kx + 1);
    }
  }

  string ans;
  REP(i, m) {
    if (T[i].first == 'a') {
      REP(j, T[i].second) {
        ans.push_back('a');
      }
      continue;
    }

    Int r = rest[i];
    if (r == kMax) {
      // z = 0
      continue;
    }
    Int z = ceil_div<Int>(X, r) - 1;
    chmin(z, T[i].second * K);
    REP(j, z) ans.push_back('b');
    X -= z * r;
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
