#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

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

using namespace std;

struct SimpleFunctionalGraph {
 private:
  static const int kMaxBits = 39;

  // number of nodes.
  int size;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

 public:
  explicit SimpleFunctionalGraph(int n)
      : size(n), next_pos(kMaxBits, std::vector<int>(n)), build_done_(false) {
    for (int d = 0; d < kMaxBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `j` as the next position of node `i`.
  void set_next(int i, int j) { next_pos[0][i] = j; }

  // Builds the transition table.
  void build() {
    for (int d = 0; d + 1 < kMaxBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
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
        i = next_pos[d][i];
      }
    }
    return i;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const i64 n = in, K = in;
  auto tele = vector(3, vector(n, 0));
  REP(i, 3) {
    REP(j, n) {
      tele[i][j] = int(in) - 1;
    }
  }
  SimpleFunctionalGraph fg(n);
  REP(j, n) {
    int v1 = tele[0][j];
    int v2 = tele[1][v1];
    int v3 = tele[2][v2];
    fg.set_next(j, v3);
  }
  fg.build();
  vector<int> ans(n);
  REP(i, n) {
    int v = fg.go(i, K / 3);
    REP(j, K % 3) {
      v = tele[j][v];
    }
    ans[i] = v + 1;
  }
  print_seq(ans);
}
