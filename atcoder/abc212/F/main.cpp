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
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

static const int kMaxBits = 60;

// Computes node transition only.
struct SimpleFunctionalGraph {
 private:
  static const int kMaxBits = 60;

  // number of nodes.
  int size;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

 public:
  explicit SimpleFunctionalGraph(int n)
      : size(n),
        next_pos(kMaxBits, std::vector<int>(n)),
        build_done_(false) {
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

  template<class F>
  long long min_steps(int start, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    assert(build_done_);
    long long max_false = 0;
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      auto j = next_pos[d][i];
      if (pred(j)) continue;
      max_false += 1LL << d;
      i = j;
    }
    return max_false + 1;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);

  INPUT(int, n, m, Q);
  vector<map<int, array<int, 3>>> departure(n);
  vector<array<int, 4>> timeloc(m);
  REP(i, m) {
    INPUT(int, a, b, s, t);
    --a, --b;
    departure[a][s] = {t, b, i};
    timeloc[i] = {s, a, t, b};
  }
  SimpleFunctionalGraph fg(m);
  REP(i, m) {
    auto[s, a, t, b] = timeloc[i];
    auto it = departure[b].lower_bound(t);
    if (it == departure[b].end()) {
      //fg.set_next(i, i);
    } else {
      auto[t2, b2, i2] = it->second;
      fg.set_next(i, i2);
    }
  }
  fg.build();

  REP(q, Q) {
    INPUT(int, x, y, z);
    --y;
    auto it = departure[y].lower_bound(x);
    if (it == departure[y].end() or it->first >= z) {
      print(y + 1);
      continue;
    }
    auto[t, b, i] = it->second;
    pair<int, int> ans = {y, b};
    if (t < z) {
      ans = {b, -1};
      for (int d = kMaxBits - 1; d >= 0; --d) {
        auto j = fg.next_pos[d][i];
        const auto&[sj, aj, tj, bj] = timeloc[j];
        if (sj >= z) continue;
        if (tj >= z) {
          ans = {aj, bj};
          break;
        }
        ans = {bj, -1};
        i = j;
      }
    }
    if (ans.second == -1) {
      print(ans.first + 1);
    } else {
      print(ans.first + 1, ans.second + 1);
    }
  }
}
