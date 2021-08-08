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

const i64 BIG = 1e16;

struct GridState {
  i64 cost;
  int r;
  int c;
  bool punched;
};

auto solve() {
  INPUT(int, H, W);
  vector<string> g(H);
  REP(i, H) {
    cin >> g[i];
  }

  auto inside = [&](int r, int c) {
    return 0 <= r and r < H and 0 <= c and c < W;
  };

  auto search_shortest_path_on_grid = [&](const pair<int, int> &start,
                                          const pair<int, int> &goal) {
    const array<int, 4> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
    auto mincost = vector(H, vector(W, vector(2, BIG)));
    deque<GridState> que;
    auto push = [&](i64 cost, int r, int c, bool punched) -> bool {
      //if (not inside(r, c)) return false;
      //if (not punched and g[r][c] == '#') return false;
      if (not chmin(mincost[r][c][punched], cost)) return false;
      GridState ns{cost, r, c, punched};
      if (punched) {
        que.push_back(ns);
      } else {
        que.push_front(ns);
      }
      return true;
    };
    assert(push(0LL, start.first, start.second, false));

    while (not que.empty()) {
      GridState cur = que.front();
      que.pop_front();
      if (cur.cost != mincost[cur.r][cur.c][cur.punched]) continue;
      if (cur.r == H - 1 and cur.c == W - 1) break;
      if (cur.punched) {
        for (int dr = -1; dr <= 1; ++dr) {
          for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 and dc == 0) continue;
            int nr = cur.r + dr;
            int nc = cur.c + dc;
            if (not inside(nr, nc)) continue;
            push(cur.cost, nr, nc, false);
          }
        }
      }
      for (int i = 0; i < 4; ++i) {
        const int nr = cur.r + dx[i];
        const int nc = cur.c + dy[i];
        if (not inside(nr, nc)) continue;
        if (g[nr][nc] == '.') {
          push(cur.cost, nr, nc, false);
        }
        push(cur.cost + 1, nr, nc, true);
      }
    }
    return mincost[goal.first][goal.second][false];
  };

  return search_shortest_path_on_grid({0, 0}, {H - 1, W - 1});
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
