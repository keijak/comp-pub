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

#ifdef ENABLE_DEBUG
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

struct State {
  int moves;
  int x;
  int y;
  int typ;
};
bool operator<(const State &a, const State &b) {
  if (a.x != b.x) return a.x < b.x;
  if (a.y != b.y) return a.y < b.y;
  return a.typ < b.typ;
}

int tflip(int typ) {
  if (typ == 1) return 3;
  if (typ == 3) return 1;
  return typ;
}

deque<State> q;
set<State> visited;

int solve(int tcase) {
  int ax, ay, bx, by, cx, cy;
  cin >> ax >> ay >> bx >> by >> cx >> cy;

  if ((ax - bx) * (ax - cx) + (ay - by) * (ay - cy) == 0) {
    swap(ax, bx);
    swap(ay, by);
  }
  if ((cx - bx) * (ax - cx) + (cy - by) * (ay - cy) == 0) {
    swap(cx, bx);
    swap(cy, by);
  }

  int init_typ = 0;

  if (bx < 0 and by < 0) {
    init_typ = 2;
  } else if (bx < 0) {
    init_typ = 1;
  } else if (by < 0) {
    init_typ = 3;
  }
  if (bx < 0) {
    ax *= -1;
    bx *= -1;
    cx *= -1;
  }
  if (by < 0) {
    ay *= -1;
    by *= -1;
    cy *= -1;
  }

  //   // Ensure bx >= by: to the right.
  //   if (bx < by) {
  //     swap(ax, ay);
  //     swap(bx, by);
  //     swap(cx, cy);
  //     init_typ = tflip(init_typ);
  //   }
  // Ensure ax <= cx
  if (ax > cx) {
    swap(ax, cx);
    swap(ay, cy);
  }

  DEBUG(ax, bx, cx);
  DEBUG(ay, by, cy);
  int goal_typ = -1;
  {
    if (ax == bx and ay == by + 1 and cx == bx + 1 and cy == by) {
      goal_typ = 0;
    } else if (ax == bx - 1 and ay == by and cx == bx and cy == by + 1) {
      goal_typ = 1;
    } else if (ax == bx - 1 and ay == by and cx == bx and cy == by - 1) {
      goal_typ = 2;
    } else {
      assert(ax == bx and ay == by - 1 and cx == bx + 1 and cy == by);
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

  int moves = 0;

  if (bx > by + 10) {
    int delta = bx - by - 10;
    bx = by + 10;
    moves += 2 * delta;
  }
  if (by > bx + 10) {
    int delta = by - bx - 10;
    by = bx + 10;
    moves += 2 * delta;
  }
  if (bx + by > 10) {
    int delta = (bx + by - 10) / 2;
    bx -= delta;
    by -= delta;
    moves += 2 * delta;
  }

  assert(bx + by < 200);
  auto search = [&](int x, int y, int typ) -> int {
    q.push_back({0, x, y, typ});
    visited.insert(q.back());
    array<State, 7> nexts;
    while (q.size()) {
      auto p = q.front();
      q.pop_front();
      // if (tcase == 0) DEBUG(p.moves, p.x, p.y, p.typ);
      if (p.x == bx and p.y == by and p.typ == goal_typ) {
        return p.moves;
      }
      next_pos(p, nexts);
      for (auto np : nexts) {
        if (visited.count(np)) continue;
        visited.insert(np);
        q.push_back(np);
      }
    }
    return -2;
  };

  q.clear();
  visited.clear();
  moves += search(0, 0, init_typ);
  return moves;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << solve(i) << '\n'; }
}
