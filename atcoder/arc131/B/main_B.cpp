#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
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

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() {
  int H = in, W = in;
  vector<string> g(H);
  REP(i, H) cin >> g[i];
  MinHeap<tuple<int, int, int>> pq;

  auto inside = [&](int x, int y) {
    return 0 <= x and x < H and 0 <= y and y < W;
  };

  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    static const int dx[4] = {1, 0, -1, 0};
    static const int dy[4] = {0, 1, 0, -1};
    std::vector<std::pair<int, int>> ret;
    for (int d = 0; d < 4; ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  REP(i, H) {
    REP(j, W) {
      if (g[i][j] != '.') continue;
      int c = 0;
      set<char> s = {'1', '2', '3', '4', '5'};
      for (auto[x, y]: adjacent(i, j)) {
        if (g[x][y] == '.') continue;
        s.erase(g[x][y]);
      }
      pq.emplace(ssize(s), i, j);
    }
  }
  while (pq.size()) {
    auto[c, i, j] = pq.top();
    pq.pop();
    if (g[i][j] != '.') continue;
    set<char> s = {'1', '2', '3', '4', '5'};
    for (auto[x, y]: adjacent(i, j)) {
      if (g[x][y] == '.') continue;
      s.erase(g[x][y]);
    }
    if (ssize(s) != c) continue;
    check(not s.empty());
    char z = *s.begin();
    g[i][j] = z;
    for (auto[x, y]: adjacent(i, j)) {
      if (g[x][y] == '.') {
        int nc = 0;
        set<char> ns = {'1', '2', '3', '4', '5'};
        for (auto[nx, ny]: adjacent(x, y)) {
          if (g[nx][ny] != '.') ns.erase(g[nx][ny]);
        }
        pq.emplace(ssize(ns), x, y);
      }
    }
  }
  REP(i, H) {
    print(g[i]);
  }
  return Void{};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
