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
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() {
  int n = in;
  vector<string> a(n, string(n, '.'));

  auto inside = [&](int x, int y) {
    return 0 <= x and x < n and 0 <= y and y < n;
  };
  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    static const int dx[] = {1, 0, -1, 0};
    static const int dy[] = {0, 1, 0, -1};
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < std::size(dx); ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  for (int i = 0, j = 0; i < n or j < n; i += 1, j += 2) {
    if (inside(i, j)) a[i][j] = 'X';
    for (int x = i, y = j; x >= 0 or y < n; x -= 2, y += 1) {
      if (inside(x, y)) a[x][y] = 'X';
    }
    for (int x = i, y = j; x < n or y >= 0; x += 2, y -= 1) {
      if (inside(x, y)) a[x][y] = 'X';
    }
  }
  REP(i, n) {
    REP(j, n) {
      if (a[i][j] == 'X') continue;
      bool ok = false;
      for (auto[x, y]: adjacent(i, j)) {
        if (a[x][y] == 'X') ok = true;
      }
      if (not ok) a[i][j] = 'X';
    }
  }
  int cnt = 0;
  REP(i, n) REP(j, n) if (a[i][j] == 'X') ++cnt;
  DUMP(cnt);
  REP(i, n) { print(a[i]); }
  return Void();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
