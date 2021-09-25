#include <bits/stdc++.h>
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
  return (int)a.size();
}

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template <typename T>
    operator T() const {
      T x(n);
      for (auto &e : x) std::cin >> e;
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

template <typename V>
void flip_vertically(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  for (int i = 0;; ++i) {
    const int r = h - 1 - i;
    if (r <= i) break;
    std::swap(grid[i], grid[r]);
  }
}

auto solve() {
  const int R = in, C = in;
  int K = int(in) - 1;
  DUMP(K);
  vector<string> G(R);
  cin.ignore();
  REP(i, R) {
    getline(cin, G[i]);
    assert(ssize(G[i]) == C);
  }

  i64 ans = 0;
  REP(j, C) {
    if (G[K][j] == 'X') ++ans;
  }
  DUMP(ans);
  REP(flipping, 2) {
    auto g = G;
    REP(shift, R) {
      int move_count = 0;
      REP(j, C) {
        REP(i, 1, R) {
          if (g[i][j] == 'X' and g[i - 1][j] != 'X') {
            g[i - 1][j] = 'X';
            g[i][j] = '.';
            ++move_count;
          }
        }
      }
      if (move_count == 0) break;
      cerr << "flip " << flipping << " shift " << shift << " K=" << K << endl;
      REP(i, R) { DUMP(i, g[i]); }
      i64 cnt = shift + 1;
      REP(j, C) if (g[K][j] == 'X')++ cnt;
      if (chmin(ans, cnt)) {
        DUMP(ans, flipping, shift);
      }
    }

    flip_vertically(G);
    K = R - 1 - K;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    cout << "Case #" << (test_case + 1) << ": ";
    auto ans = solve();
    print(ans);
  }
}
