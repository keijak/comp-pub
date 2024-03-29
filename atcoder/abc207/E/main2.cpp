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
  read_from_cin(__VA_ARGS__);

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

#include <boost/multiprecision/cpp_int.hpp>
//using Int = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
//using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

using Int = i64; // for debug.

struct KV {
  int key;
  Mint val;
};
bool operator<(const KV &x, const KV &y) {
  return x.key < y.key;
}

auto solve() {
  INPUT(int, n);
  vector<Int> a(n), cum(n + 1);
  REP(i, n) {
    i64 x;
    cin >> x;
    a[i] = x;
    cum[i + 1] = cum[i] + x;
  }

  auto ix = vector(n + 1, vector(n, vector<int>()));
  for (int k = 1; k <= n; ++k) {
    for (int i = 1; i <= n; ++i) {
      Int m = cum[i] % k;
      ix[k][(int) m].push_back(i);
    }
  }

  auto dp = vector(n + 1, vector(n + 1, Mint()));
  auto dp2 = vector(n + 1, vector(n + 1, vector<Mint>()));

  for (int i = 0; i <= n; ++i) {
    dp[1][i] = 1;
  }
  for (int k = 2; k <= n; ++k) {
    for (int m = 0; m < k; ++m) {
      auto &memo = dp2[k][m];
      int sz = ix[k][m].size();
      memo.resize(sz);
      Mint s = 0;
      REP(j, sz) {
        int p = ix[k][m][j];
        auto sub = dp[k - 1][p];
        memo[j] = s + sub;
        s += sub;
      }
    }
    for (int r = 1; r <= n; ++r) {
      int m = int(cum[r] % k);
      Mint res = 0;
      int j = lower_bound(ALL(ix[k][m]), r) - ix[k][m].begin();
      if (j > 0) {
        --j;
        res += dp2[k][m][j];
      }
      dp[k][r] = res;
    }
  }
  Mint ans = 0;
  for (int i = 1; i <= n; ++i) {
    auto sub = dp[i][n];
    ans += sub;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
