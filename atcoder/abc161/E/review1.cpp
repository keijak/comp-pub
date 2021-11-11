#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
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
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

auto solve() {
  int n = in, K = in, C = in;
  string S = in;
  auto dpr = vector(n + 1, 0LL);
  for (int i = n - 1; i >= 0; --i) {
    dpr[i] = dpr[i + 1];
    if (S[i] == 'x') continue;
    if (i + C + 1 <= n) {
      chmax(dpr[i], dpr[i + C + 1] + 1);
    } else {
      chmax(dpr[i], 1);
    }
  }
  auto dpf = vector(n + 1, 0LL);
  for (int i = 1; i <= n; ++i) {
    dpf[i] = dpf[i - 1];
    if (S[i - 1] == 'x') continue;
    if (i - C - 1 >= 0) {
      chmax(dpf[i], dpf[i - C - 1] + 1);
    } else {
      chmax(dpf[i], 1);
    }
  }
  DUMP(dpr);
  DUMP(dpf);
  vector<int> ok(n, 0);
  REP(i, n) {
    if (dpf[i] >= K or dpr[i + 1] >= K) ok[i] = true;
  }
  vector<Int> cum(n + 10, 0);
  REP(l, n) {
    int r = clamp(l + C, l + 1, n);
    Int c = dpf[l] + dpr[r];
    if (c >= K) {
      cum[l] += 1;
      cum[r] -= 1;
    }
  }
  for (int i = 1; i <= n; ++i) {
    cum[i] += cum[i - 1];
  }
  REP(i, n) {
    if (cum[i] > 0) ok[i] = true;
  }
  REP(i, n) {
    if (S[i] == 'o' and not ok[i]) {
      print(i + 1);
    }
  }
  return Void{};
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
