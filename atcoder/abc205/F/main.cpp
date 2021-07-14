#include <bits/stdc++.h>

#include <atcoder/maxflow>

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

auto solve() {
  int H, W, n;
  cin >> H >> W >> n;

  atcoder::mf_graph<int> g(H + W + n * 2 + 2);
  int kSource = H + W + n * 2;
  int kSink = H + W + n * 2 + 1;

  for (int x = 0; x < H; ++x) {
    int row_node = x;
    g.add_edge(kSource, row_node, 1);
  }
  for (int y = 0; y < W; ++y) {
    int col_node = H + y;
    g.add_edge(col_node, kSink, 1);
  }

  REP(i, n) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    --a, --b, --c, --d;
    int u = H + W + i * 2;
    int v = H + W + i * 2 + 1;
    g.add_edge(u, v, 1);
    for (int x = a; x <= c; ++x) {
      int row_node = x;
      g.add_edge(row_node, u, 1);
    }
    for (int y = b; y <= d; ++y) {
      int col_node = H + y;
      g.add_edge(v, col_node, 1);
    }
  }
  return g.flow(kSource, kSink, n);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
