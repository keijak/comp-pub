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
  for (auto &x: a) is >> x;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() {
  const int n = in, K = in;
  vector<vector<int>> g(n), bk(n + 1);
  vector<i64> a(n);
  REP(i, n) {
    a[i] = in;
    int m = in;
    REP(j, m) {
      int b = in;
      --b;
      g[b].push_back(i);
      bk[i].push_back(b);
    }
  }

  auto dp = vector(n + 1, vector(n + 1, vector(K + 1, vector(n, optional<i64>()))));
  auto f = [&](auto &f, int l, int r, int k, int p) -> i64 {
    if (l == r) return 0;
    if (dp[l][r][k][p]) return *dp[l][r][k][p];
    i64 ret = chmax(ret, f(f, l, r - 1, k, bk[r - 1].size()));
    if (k > 0) {
      for (int u = 0; u < p; ++u) {
        // l -> j -> r
        int j = bk[r][u];
        assert(j < r);
        if (j < l) continue;
        i64 gain = a[r] - a[j];
        if (gain <= 0) continue;
        i64 sub1 = 0;
        if (l < j) {
          sub1 += f(f, l, j, k, bk[j].size());
        }
        i64 sub2 = 0;
        if (j < r) {
          sub2 = f(f, j, r, k - 1, u);
        }
        chmax(ret, sub1 + sub2 + gain);
      }
    }
    dp[l][r][k][p] = ret;
    return ret;
  };
  return f(f, 0, n - 1, K, bk[n - 1].size());
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
