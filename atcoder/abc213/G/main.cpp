#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

inline int popcount(unsigned x) { return __builtin_popcount(x); }

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  INPUT(int, n, m);
  auto g = vector(n, vector(n, 0));
  vector<pair<int, int>> edges(m);
  REP(i, m) {
    INPUT(int, a, b);
    --a, --b;
    g[a][b] = g[b][a] = true;
    edges[i] = {a, b};
  }

  vector<Mint> pow2(m + 1, 1);
  REP(i, 1, m + 1) {
    pow2[i] = pow2[i - 1] * 2;
  }

  auto es = vector(1 << n, 0);
  REP(bits, 1 << n) {
    int count = 0;
    for (auto[u, v] : edges) {
      if (not(bits & (1 << v))) continue;
      if (not(bits & (1 << u))) continue;
      ++count;
    }
    es[bits] = count;
  }

  const unsigned kFull = (1 << n) - 1;
  auto dp = vector(1 << n, Mint(0));

  for (unsigned bits = 1; bits <= kFull; bits += 2) {
    dp[bits] = pow2[es[bits]];
    for (unsigned sub = (bits - 1) & bits; sub; sub = (sub - 1) & bits) {
      if (not(sub & 1)) continue;
      unsigned other = (bits ^ sub);
      dp[bits] -= dp[sub] * pow2[es[other]];
    }
  }

  vector<Mint> ans(n, 0);
  REP(bits, 1 << n) {
    if (not(bits & 1)) continue;
    REP(x, 1, n) {
      if (bits & (1 << x)) {
        unsigned other = kFull ^ bits;
        ans[x] += dp[bits] * pow2[es[other]];
      }
    }
  }
  REP(i, 1, n) {
    print(ans[i]);
  }
}
