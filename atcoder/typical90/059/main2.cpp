#include <bits/stdc++.h>

#include <atcoder/scc>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

#include <boost/multiprecision/cpp_int.hpp>
using Uint = boost::multiprecision::uint512_t;
const int B = 512;

void solve() {
  int n, m, Q;
  cin >> n >> m >> Q;
  vector<vector<int>> edges(n);

  REP(i, m) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    edges[x].push_back(y);
  }
  REP(i, n) {
    sort(ALL(edges[i]));
    edges[i].erase(unique(ALL(edges[i])), edges[i].end());
  }

  for (int i = 0; i < Q; i += B) {
    vector<Uint> ea(n, 0), eb(n, 0);
    // auto events = vector(n, vector(2, Uint(0)));
    vector<Uint> activeq(n, 0);
    // fill(ALL(events), 0);
    // fill(ALL(activeq), 0);
    const int blen = min(B, Q - i);
    REP(j, blen) {
      int a, b;
      cin >> a >> b;
      assert(a < b);
      --a, --b;
      eb[b] |= Uint(1) << j;
      ea[a] |= Uint(1) << j;
    }

    Uint ans = 0;
    for (int u = 0; u < n; ++u) {
      auto &aq = activeq[u];
      if (const auto &x = eb[u]; x) {
        ans |= aq & x;
      }
      if (const auto &x = ea[u]; x) {
        aq |= x;
      }
      if (aq) {
        for (auto v : edges[u]) {
          activeq[v] |= aq;
        }
      }
    }

    REP(j, blen) {
      if (ans >> j & 1) {
        cout << "Yes\n";
      } else {
        cout << "No\n";
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
