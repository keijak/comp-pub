// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &output_seq(
    const Container &seq, const char *sep = " ", const char *ends = "\n",
    std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &output_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &output(const T &x) {
  return output_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &output(const T &head, Ts... tail) {
  return output_one(head, ' '), output(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() {
  vector<vector<string>> T(12);
  T[5] = {
      {".#.##"},
      {"#.#.#"},
      {"#.##."},
      {".#.##"},
      {"###.."},
  };
  T[6] = {
      {"###..."},
      {"...###"},
      {"###..."},
      {"...###"},
      {"###..."},
      {"...###"},
  };
  T[7] = {
      {"##.#..."},
      {"..###.."},
      {"...#.##"},
      {"###...."},
      {"....###"},
      {"###...."},
      {"....###"},
  };
  T[8] = {
      {"##.#...."},
      {"..###..."},
      {".....###"},
      {"###....."},
      {"....#.##"},
      {"...###.."},
      {"###....."},
      {".....###"},
  };
  T[9] = {
      {"#......##"},
      {".#.#...#."},
      {".#.##...."},
      {".#..##..."},
      {"....###.."},
      {"...#..##."},
      {"#.#...#.."},
      {"#.#.....#"},
      {"..#..#..#"},
  };
  T[10] = {
      {"###......."},
      {"...#..#.#."},
      {"...##...#."},
      {"..#.....##"},
      {".....##..#"},
      {".#..##...."},
      {"###......."},
      {"#......#.#"},
      {".....###.."},
      {"...##..#.."},
  };
  T[11] = vector(11, string(11, '.'));
  REP(i, 6) REP(j, 6) T[11][i][j] = T[6][i][j];
  REP(i, 5) REP(j, 5) T[11][6 + i][6 + j] = T[5][i][j];
  const int n = in;
  int q = n / 6;
  int r = n % 6;
  DUMP(n, q, r);
  vector<string> g(n, string(n, '.'));
  REP(i, q - 1) {
    for (int x = 6 * i; x < 6 * (i + 1); ++x) {
      for (int y = 6 * i; y < 6 * (i + 1); ++y) {
        g[x][y] = T[6][x % 6][y % 6];
      }
    }
  }
  {
    vector<string> &t = T[6 + r];
    for (int x = 6 * (q - 1); x < n; ++x) {
      for (int y = 6 * (q - 1); y < n; ++y) {
        g[x][y] = t[x - 6 * (q - 1)][y - 6 * (q - 1)];
      }
    }
  }
  for (const auto &s: g) {
    output(s);
  }
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
