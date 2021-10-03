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

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;


struct Diameter {
  int distance;
  std::array<int, 2> nodes;
};

// [1, x, x^2, x^3, ..., x^n]
template<typename T>
std::vector<T> pow_seq(int n, int base = 2) {
  assert(n >= 0 and base > 0);
  std::vector<T> p(n + 1);
  p[0] = 1;
  for (int i = 0; i < n; ++i) {
    p[i + 1] = p[i] * base;
  }
  return p;
}

auto solve() {
  const int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  vector<int> par(n, -1);
  vector<int> depth(n, -1);

  auto farthest = [&](auto &rec, int v, int p, int d) -> std::pair<int, int> {
    int res_d = d, res_v = v;
    par[v] = p;
    depth[v] = d;
    for (auto u: g[v]) {
      if (u == p) continue;
      const auto &[child_d, child_v] = rec(rec, u, v, d + 1);
      if (child_d > res_d) {
        res_d = child_d;
        res_v = child_v;
      }
    }
    return {res_d, res_v};
  };

  const auto &[_, l0] = farthest(farthest, 0, -1, 0);
  const auto &[D, r0] = farthest(farthest, l0, -1, 0);
  vector<int> dpath;
  for (int p = r0;; p = par[p]) {
    dpath.push_back(p);
    if (p == l0) break;
  }
  reverse(ALL(dpath));

  vector<pair<int, int>> subroots;
  if (D % 2 == 1) {
    int lc = dpath[(D - 1) / 2];
    int rc = dpath[(D + 1) / 2];
    subroots.emplace_back(lc, rc);
    subroots.emplace_back(rc, lc);
  } else {
    int center = dpath[D / 2];
    for (auto v: g[center]) {
      subroots.emplace_back(v, center);
    }
  }

  auto f = [&](auto &f, int v, int p, int d, int dt) -> int {
    if (d == dt) return 1;
    int ret = 0;
    for (auto u: g[v]) {
      if (u == p) continue;
      ret += f(f, u, v, d + 1, dt);
    }
    return ret;
  };
  vector<int> cs;
  for (auto[sr, sp]: subroots) {
    int d = (D % 2 == 1) ? 0 : 1;
    int c = f(f, sr, sp, d, D / 2);
    cs.push_back(c);
  }

  Mint ans = 1;
  for (auto c: cs) {
    ans *= c + 1;
  }
  for (auto c: cs) {
    ans -= c;
  }
  return ans - 1;
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
