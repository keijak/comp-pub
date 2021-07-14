#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Float = long double;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
const Float EPS = 1e-10;

Float solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(n);
  REP(i, m) {
    int s, t;
    cin >> s >> t;
    --s, --t;
    assert(s < t);
    g[s].push_back(t);
  }
  assert(g[n - 1].empty());
  assert(ssize(g[n - 2]) == 1);
  assert(g[n - 2][0] == n - 1);
  auto exp = vector(n, (Float)0.0);
  for (int v = n - 2; v >= 0; --v) {
    int k = ssize(g[v]);
    Float e = 0.0;
    for (auto u : g[v]) {
      e += 1 + exp[u];
    }
    e /= k;
    exp[v] = e;
  }
  DUMP(exp);
  Float ans = exp[0];
  const int k0 = ssize(g[0]);

  if (ssize(g[0]) >= 2) {
    Float ma = 0.0;
    for (auto u : g[0]) {
      chmax(ma, 1 + exp[u]);
    }
    Float e1 = (exp[0] - ma / k0) * Float(k0) / (k0 - 1);
    DUMP(e1);
    chmin(ans, e1);
  }

  vector<Float> a(n, 0.0);
  for (auto u : g[0]) {
    a[u] = Float(1.0) / k0;
  }

  for (int v = 0; v < n - 1; ++v) {
    if (a[v] == 0.0) continue;
    const int k = ssize(g[v]);
    Float ma = 0.0;
    Float s = 0.0;
    for (auto u : g[v]) {
      chmax(ma, 1 + exp[u]);
      s += 1 + exp[u];
    }
    if (k >= 2) {
      Float eb = (s - ma) / (k - 1);
      Float e = exp[0] + (eb - exp[v]) * a[v];
      chmin(ans, e);
    }
    for (auto u : g[v]) {
      a[u] += a[v] / k;
    }
    a[v] = 0.0;
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  cout << solve() << "\n";
}
