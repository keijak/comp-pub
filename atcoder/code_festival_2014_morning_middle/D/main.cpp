#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
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

using namespace std;

auto solve() {
  const int n = in;
  vector<i64> pos(n), len(n);
  REP(i, n) {
    pos[i] = i64(in);
    len[i] = i64(in);
  }
  DUMP(pos);
  DUMP(len);
  auto dp = vector(n, vector(2020, Mint(0)));
  const i64 kOffset = 1010;
  for (int x = -len[0]; x <= len[0]; ++x) {
    dp[0][kOffset + x] = 1;
  }
  REP(i, 1, n) {
    vector<Mint> acc(2021);
    REP(x, 2020) acc[x + 1] = acc[x] + dp[i - 1][x];

    for (int y = -len[i]; y <= len[i]; ++y) {
      i64 r = min<i64>(kOffset + pos[i] + y - pos[i - 1], 2020);
      i64 l = max<i64>(kOffset - len[i - 1] - 1, 0);
      dp[i][kOffset + y] += acc[r] - acc[l];

//      for (int x = kOffset - len[i - 1]; x <= kOffset + len[i - 1]; ++x) {
//        if (x < kOffset + pos[i] + y - pos[i - 1]) {
//          dp[i][kOffset + y] += dp[i - 1][x];
//        }
//      }
    }
  }
  return accumulate(ALL(dp[n - 1]), Mint(0));
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  print(ans);
}
