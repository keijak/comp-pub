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

template<class T>
struct CumSum2d {
  template<class U>
  using Vec = std::vector<U>;

  int n, m;
  Vec<Vec<T>> cum;

  template<class U>
  explicit CumSum2d(const Vec<Vec<U>> &data)
      : n(data.size()), m(data.at(0).size()), cum(n + 1, Vec<T>(m + 1, T(0))) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cum[i + 1][j + 1] =
            cum[i][j + 1] + cum[i + 1][j] - cum[i][j] + data[i][j];
      }
    }
  }

  // (r0, c0) := top-left point (inclusive)
  // (r1, c1) := bottom-right point (exclusive)
  T sum(int r0, int c0, int r1, int c1) const {
    assert(r0 <= r1 and c0 <= c1);
    return cum[r1][c1] - cum[r1][c0] - cum[r0][c1] + cum[r0][c0];
  }
};

using namespace std;

auto solve() {
  const int n = in, m = in;
  vector g(n, vector(m, 0));
  REP(i, n) {
    string row = in;
    REP(j, m) {
      if (row[j] == '1') g[i][j] = 1;
    }
  }
  CumSum2d<int> cg(g);
  i64 ans = 16;
  REP(i1, n) {
    if (i1 + 5 > n) break;
    REP(j1, m) {
      if (j1 + 4 > m) break;
      for (int i2 = i1 + 5; i2 <= n; ++i2) {
        int j2 = j1 + 4;
        //DUMP(i1 + 1, j1 + 1, i2 - 1, j2 - 1, n, m);
        int inner0 = cg.sum(i1 + 1, j1 + 1, i2 - 1, j2 - 1);
        if (inner0 >= ans) break;
        while (j2 <= m) {
          inner0 = cg.sum(i1 + 1, j1 + 1, i2 - 1, j2 - 1);
          i64 b1 = (j2 - j1 - 2) - cg.sum(i1, j1 + 1, i1 + 1, j2 - 1);
          i64 b3 = (i2 - i1 - 2) - cg.sum(i1 + 1, j1, i2 - 1, j1 + 1);
          if (inner0 + b1 + b3 >= ans) break;
          i64 b2 = (j2 - j1 - 2) - cg.sum(i2 - 1, j1 + 1, i2, j2 - 1);
          i64 b4 = (i2 - i1 - 2) - cg.sum(i1 + 1, j2 - 1, i2 - 1, j2);
          i64 ret = inner0 + b1 + b2 + b3 + b4;
          chmin(ans, ret);
          if (j2 == m) break;
          if (cg.sum(i1 + 1, j1 + 1, i2 - 1, m) <= inner0) break;
          int ng = j2, ok = m;
          while (ok - ng > 1) {
            int mid = (ok + ng) / 2;
            int inner = cg.sum(i1 + 1, j1 + 1, i2 - 1, mid);
            if (inner > inner0) {
              ok = mid;
            } else {
              ng = mid;
            }
          }
          j2 = ok;
        }
      }
    }
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int t = in;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
