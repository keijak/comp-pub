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

const i64 BIG = 1e16;

struct GridState {
  i64 cost;
  int r;
  int c;
  int dir;
};

auto solve() {
  const array<int, 4> dr = {-1, 0, 1, 0};
  const array<int, 4> dc = {0, -1, 0, 1};
  int H, W, rs, cs, rt, ct;
  cin >> H >> W >> rs >> cs >> rt >> ct;
  --rs, --cs, --rt, --ct;
  vector<string> cell(H);
  REP(i, H) cin >> cell[i];

  auto mincost = vector(H, vector(W, vector(4, BIG)));
  deque<GridState> que;
  REP(d, 4) {
    mincost[rs][cs][d] = 0;
    que.push_back({0, rs, cs, d});
  }

  auto push = [&](i64 cost, int r, int c, int dir_from, int dir_to) -> bool {
    if (r < 0 or r >= H or c < 0 or c >= W) return false;
    if (cell[r][c] == '#') return false;
    i64 new_cost = cost + (dir_from == dir_to ? 0 : 1);
    if (not chmin(mincost[r][c][dir_to], new_cost)) {
      return false;
    }
    if (dir_from == dir_to) {
      que.push_front({new_cost, r, c, dir_to});
    } else {
      que.push_back({new_cost, r, c, dir_to});
    }
    return true;
  };

  while (not que.empty()) {
    GridState cur = move(que.front());
    que.pop_front();
    if (cur.cost != mincost[cur.r][cur.c][cur.dir]) {
      continue;
    }
    if (cur.r == rt and cur.c == ct) {
      return cur.cost;
    }
    assert(cell[cur.r][cur.c] == '.');
    for (int i = 0; i < 4; ++i) {
      const int nr = cur.r + dr[i];
      const int nc = cur.c + dc[i];
      push(cur.cost, nr, nc, cur.dir, i);
    }
  }
  return BIG;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
