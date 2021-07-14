#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

const i64 BIG = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct GridState {
  i64 cost;
  int r;
  int c;
  bool down;
};
bool operator>(const GridState &x, const GridState &y) {
  if (x.cost != y.cost) return x.cost > y.cost;
  if (not x.down and y.down) return true;
  return false;
}

i64 solve() {
  int R, C;
  cin >> R >> C;
  vector A(R, vector(C - 1, 0));
  vector B(R - 1, vector(C, 0));
  REP(i, R) REP(j, C - 1) cin >> A[i][j];
  REP(i, R - 1) REP(j, C) cin >> B[i][j];

  vector mincost(R, vector(C, vector(2, BIG)));
  MinHeap<GridState> que;
  auto push = [&](i64 cost, int r, int c, bool down) -> bool {
    if (r < 0 or r >= R or c < 0 or c >= C) return false;
    if (not chmin(mincost[r][c][down], cost)) return false;
    que.push({cost, r, c, down});
    return true;
  };
  assert(push(0LL, 0, 0, false));

  while (not que.empty()) {
    GridState cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.r][cur.c][cur.down]) {
      continue;
    }
    if (cur.r == R - 1 and cur.c == C - 1) {
      break;
    }
    if (cur.c < C - 1) {
      i64 new_cost = cur.cost + A[cur.r][cur.c];
      push(new_cost, cur.r, cur.c + 1, false);
    }
    if (cur.c > 0) {
      i64 new_cost = cur.cost + A[cur.r][cur.c - 1];
      push(new_cost, cur.r, cur.c - 1, false);
    }
    if (cur.r < R - 1) {
      i64 new_cost = cur.cost + B[cur.r][cur.c];
      push(new_cost, cur.r + 1, cur.c, false);
    }
    if (cur.r > 0) {
      i64 new_cost = cur.cost + (cur.down ? 1 : 2);
      push(new_cost, cur.r - 1, cur.c, true);
    }
  }
  return mincost[R - 1][C - 1][0];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
