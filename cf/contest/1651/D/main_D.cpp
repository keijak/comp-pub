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
  vector<pair<int, int>> xy(n);
  map<pair<int, int>, int> xymap;

  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, 1, 0, -1};

  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < std::size(dx); ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  REP(i, n) {
    int x = in, y = in;
    xy[i] = {x, y};
    xymap[{x, y}] = i;
  }

  queue<tuple<int, int, int, int, int>> que;
  REP(i, n) {
    auto[x, y] = xy[i];
    for (auto[nx, ny]: adjacent(x, y)) {
      if (xymap.count({nx, ny})) continue;
      que.emplace(nx, ny, 0, nx, ny);
    }
  }
  vector<optional<pair<int, int>>> ans(n);
  while (not que.empty()) {
    auto[x, y, d, x0, y0] = que.front();
    que.pop();
    for (auto[nx, ny]: adjacent(x, y)) {
      auto it = xymap.find({nx, ny});
      if (it == xymap.end()) continue;
      if (ans[it->second].has_value()) continue;
      ans[it->second] = {x0, y0};
      que.emplace(nx, ny, d + 1, x0, y0);
    }
  }
  REP(i, n) {
    auto[x, y] = ans[i].value();
    print(x, y);
  }
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
