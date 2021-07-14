#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::checked_int128_t;

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

struct SimpleFunctionalGraph {
 private:
  static const int kMaxBits = 60;

  // number of nodes.
  int size;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<std::optional<int>>> next_pos;

  bool build_done_;

 public:
  explicit SimpleFunctionalGraph(int n)
      : size(n),
        next_pos(kMaxBits, std::vector<std::optional<int>>(n)),
        build_done_(false) {}

  // Sets `v` as the next position of node `u`.
  void set_next(int u, int v) { next_pos[0][u] = v; }

  // Builds the transition table.
  void build() {
    for (int d = 0; d + 1 < kMaxBits; ++d) {
      for (int i = 0; i < size; ++i) {
        if (const auto &p = next_pos[d][i]; p.has_value()) {
          next_pos[d + 1][i] = next_pos[d][p.value()];
        }
      }
    }
    build_done_ = true;
  }

  // Starting from `start`, `steps` times goes forward and returns where it
  // ends up.
  int go(int start, long long steps) const {
    assert(build_done_);
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1LL << kMaxBits));

    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        i = next_pos[d][i].value();
      }
    }
    return i;
  }

  long long min_steps(int start, std::function<bool(int)> pred) const {
    long long max_false = 0;
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      const int j = next_pos[d][i].value();
      if (pred(j)) continue;
      max_false += 1LL << d;
      i = j;
    }
    return max_false + 1;
  }
};

int dsum(int x) {
  int ret = 0;
  while (x) {
    ret += x % 10;
    x /= 10;
  }
  return ret;
}

int solve() {
  const int L = 100000;
  int n;
  i64 k;
  cin >> n >> k;
  SimpleFunctionalGraph g(L);
  REP(x, L) {
    int y = dsum(x);
    int nex = (x + y) % L;
    g.set_next(x, nex);
  }
  g.build();
  return g.go(n, k);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
