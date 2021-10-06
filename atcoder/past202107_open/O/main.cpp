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
#endif

using namespace std;

tuple<i64, int, i64> dp[200005];

auto solve() -> i64 {
  const i64 N = in;
  vector<i64> maxb;
  vector<i64> cuma;
  maxb.reserve(N);
  cuma.reserve(N + 1);
  cuma.push_back(0);
  i64 acum = 0;
  REP(i, N) {
    i64 a = in, b = in;
    if (i > 0 and maxb.back() >= b) {
      acum += a;
      continue;
    }
    a += acum;
    acum = 0;
    maxb.push_back(b);
    cuma.push_back(cuma.back() + a);
  }
  cuma.push_back(cuma.back() + acum);
  const int Q = ssize(maxb);

  int head = 0, tail = 1;
  dp[head] = tuple{0LL, -1, 0LL};
  REP(i, Q) {
    const auto b = maxb[i];
    for (; head < tail; ++head) {
      const auto&[power, t0, stock] = dp[head];
      if (power >= b) break;
      while (tail <= Q) {
        const i64 bx = maxb[tail - 1];
        const i64 stockx = stock + cuma[i + 1] - cuma[t0 + 1] - bx;
        if (stockx < 0) break;
        dp[tail++] = {bx, i, stockx};;
      }
    }
    if (tail == Q + 1) break;
  }
  if (tail != Q + 1) return -1;
  auto[_, rj, stock] = dp[tail - 1];
  DUMP(rj, stock);
  DUMP(cuma.back(), cuma[rj + 1]);
  return stock + cuma.back() - cuma[rj + 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  print(solve());
}
