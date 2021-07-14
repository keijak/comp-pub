#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;
using i128 = __int128_t;

i64 majority(i64 a, i64 b, i64 c) {
  if (a == b or a == c) return a;
  return b;
}

struct State {
  int moves;
  i64 x;
  i64 y;
  int typ;
};
bool operator<(const State &a, const State &b) {
  if (a.x != b.x) return a.x < b.x;
  if (a.y != b.y) return a.y < b.y;
  return a.typ < b.typ;
}

i64 solve(int tcase) {
  i64 ax, ay, bx, by, cx, cy;
  cin >> ax >> ay >> bx >> by >> cx >> cy;
  i64 gx = majority(ax, bx, cx), gy = majority(ay, by, cy);
  i64 mx = ax + bx + cx - 3 * gx;
  i64 my = ay + by + cy - 3 * gy;
  int init_typ = 0;

  DEBUG(gx, gy, mx, my);
  int goal_typ = -1;
  {
    if (mx == 1 and my == 1) {
      goal_typ = 0;
    } else if (mx == -1 and my == 1) {
      goal_typ = 1;
    } else if (mx == -1 and my == -1) {
      goal_typ = 2;
    } else {
      assert(mx == 1 and my == -1);
      goal_typ = 3;
    }
  }
  DEBUG(goal_typ);

  auto next_pos = [&](const State &s, array<State, 7> &res) {
    int m = s.moves + 1;
    if (s.typ == 0) {
      res[0] = {m, s.x + 1, s.y + 1, 2};
      res[1] = {m, s.x + 1, s.y, 2};
      res[2] = {m, s.x, s.y + 1, 2};
      res[3] = {m, s.x, s.y, 1};
      res[4] = {m, s.x, s.y, 3};
      res[5] = {m, s.x + 1, s.y, 1};
      res[6] = {m, s.x, s.y + 1, 3};
    } else if (s.typ == 1) {
      res[0] = {m, s.x - 1, s.y + 1, 3};
      res[1] = {m, s.x - 1, s.y, 3};
      res[2] = {m, s.x, s.y + 1, 3};
      res[3] = {m, s.x, s.y, 0};
      res[4] = {m, s.x, s.y, 2};
      res[5] = {m, s.x - 1, s.y, 0};
      res[6] = {m, s.x, s.y + 1, 2};
    } else if (s.typ == 2) {
      res[0] = {m, s.x - 1, s.y - 1, 0};
      res[1] = {m, s.x - 1, s.y, 0};
      res[2] = {m, s.x, s.y - 1, 0};
      res[3] = {m, s.x, s.y, 3};
      res[4] = {m, s.x, s.y, 1};
      res[5] = {m, s.x - 1, s.y, 3};
      res[6] = {m, s.x, s.y - 1, 1};
    } else {
      res[0] = {m, s.x + 1, s.y - 1, 1};
      res[1] = {m, s.x + 1, s.y, 1};
      res[2] = {m, s.x, s.y - 1, 1};
      res[3] = {m, s.x, s.y, 0};
      res[4] = {m, s.x, s.y, 2};
      res[5] = {m, s.x + 1, s.y, 2};
      res[6] = {m, s.x, s.y - 1, 0};
    }
    return res;
  };

  auto search = [&](int x, int y, int typ, int max_moves) -> set<State> {
    deque<State> q;
    set<State> visited;
    State s0 = {0, x, y, typ};
    visited.insert(s0);
    q.push_back(s0);
    array<State, 7> nexts;
    while (q.size()) {
      auto p = q.front();
      q.pop_front();
      if (p.moves > max_moves) continue;
      next_pos(p, nexts);
      for (const auto &np : nexts) {
        if (visited.count(np)) continue;
        visited.insert(np);
        q.push_back(np);
      }
    }
    return visited;
  };

  auto start_pos = search(0, 0, init_typ, 5);
  auto goal_pos = search(gx, gy, goal_typ, 5);
  i64 ans = 5e18;
  for (auto s1 : start_pos) {
    for (auto s2 : goal_pos) {
      if (s1.typ != s2.typ) continue;
      if (s1.x == s2.x and s1.y == s2.y) {
        chmin(ans, i64(s1.moves + s2.moves));
        continue;
      }
      if (((s1.typ == 1 or s1.typ == 3) and
           not((s2.x - s1.x < 0) ^ (s2.y - s1.y < 0))) or
          ((s1.typ == 0 or s1.typ == 2) and
           ((s2.x - s1.x < 0) ^ (s2.y - s1.y < 0)))) {
        i64 dx = abs(s1.x - s2.x);
        i64 dy = abs(s1.y - s2.y);
        i64 d = min(dx, dy) * 2;
        d += (max(dx, dy) - min(dx, dy)) * 2;
        chmin(ans, s1.moves + s2.moves + d);
      }
    }
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << solve(i) << '\n'; }
}
