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

// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > T(1)) {
    auto mid = (truthy & falsy) + (truthy ^ falsy) / T(2);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() {
  Int R = in;
  array<int, 2> W, H, X, Y;
  array<vector<vector<int>>, 2> L;
  REP(i, 2) {
    cin >> W[i] >> H[i] >> X[i] >> Y[i];
    --X[i], --Y[i];
    L[i] = vector(H[i], vector<int>(W[i], 0));
    REP(y, H[i]) {
      REP(x, W[i]) {
        cin >> L[i][y][x];
      }
    }
    assert(L[i][Y[i]][X[i]] == 1);
  }

  auto calc = [&](int k) {
    auto inside = [&](int y, int x) {
      return 0 <= y and y < H[k] and 0 <= x and x < W[k];
    };
    auto adjacent = [&](int y, int x) -> std::vector<std::pair<int, int>> {
      static const int dx[] = {1, 0, -1, 0};
      static const int dy[] = {0, 1, 0, -1};
      std::vector<std::pair<int, int>> ret;
      for (size_t d = 0; d < std::size(dx); ++d) {
        int ny = y + dy[d];
        int nx = x + dx[d];
        if (not inside(ny, nx)) continue;
        ret.emplace_back(ny, nx);
      }
      return ret;
    };

    MinHeap<tuple<Int, int, int>> heap;
    heap.emplace(L[k][Y[k]][X[k]], Y[k], X[k]);
    heap.emplace(kBigVal<Int>, 0, 0);
    Int cur_level = 0;
    int visit = 0;
    vector<pair<Int, int>> visits_level;
    auto seen = vector(H[k], vector(W[k], 0));
    seen[Y[k]][X[k]] = true;
    while (heap.size()) {
      auto[level, y, x] = heap.top();
      heap.pop();
      if (level > cur_level) {
        visits_level.emplace_back(visit, cur_level);
        if (level == kBigVal<Int>) break; // sentinel
        cur_level = level;
      }
      ++visit;
      for (auto[ny, nx]: adjacent(y, x)) {
        if (seen[ny][nx]) continue;
        seen[ny][nx] = true;
        heap.emplace(L[k][ny][nx], ny, nx);
      }
    }
    return visits_level;
  };

  auto vl0 = calc(0);
  auto vl1 = calc(1);
  DUMP(vl0);
  DUMP(vl1);
  int r = 0;
  Int ans = kBigVal<Int>;
  for (int i = ssize(vl0) - 1; i >= 0; --i) {
    auto[visit0, level0] = vl0[i];
    while (r < ssize(vl1) and vl1[r].first + visit0 < R) ++r;
    if (r == ssize(vl1)) break;
    Int level1 = vl1[r].second;
    chmin(ans, level0 + level1);
  }
  return ans;
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
