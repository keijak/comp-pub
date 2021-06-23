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

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template <typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__);

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct UndoableUnionFind {
  std::vector<int> parent_;
  std::stack<std::pair<int, int>> history_;

  explicit UndoableUnionFind(int sz) { parent_.assign(sz, -1); }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    history_.emplace(x, parent_[x]);
    history_.emplace(y, parent_[y]);
    if (x == y) return (false);
    if (parent_[x] > parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return (true);
  }

  int find(int k) {
    if (parent_[k] < 0) return (k);
    return (find(parent_[k]));
  }

  int size(int k) { return (-parent_[find(k)]); }

  void undo() {
    parent_[history_.top().first] = history_.top().second;
    history_.pop();
    parent_[history_.top().first] = history_.top().second;
    history_.pop();
  }

  void snapshot() {
    while (history_.size()) history_.pop();
  }

  void rollback() {
    while (history_.size()) undo();
  }
};

const std::array<int, 4> dx = {1, 0, -1, 0};
const std::array<int, 4> dy = {0, 1, 0, -1};

auto solve() {
  const int n = 10;
  const int nn = n * n;
  vector<string> g(n);
  REP(i, n) { cin >> g[i]; }

  UndoableUnionFind uf(nn);

  int island = -1;
  int ic = 0;
  REP(i, n) REP(j, n) {
    if (g[i][j] != 'o') continue;
    ++ic;
    island = i * n + j;
    REP(d, 4) {
      int ni = i + dx[d];
      int nj = j + dy[d];
      if (ni < 0 or ni >= n or nj < 0 or nj >= n) continue;
      if (g[ni][nj] == 'o') {
        uf.unite(i * n + j, ni * n + nj);
      }
    }
  }
  assert(island >= 0);
  if (uf.size(island) == ic) return true;  // already connected
  REP(i, n) REP(j, n) {
    if (g[i][j] == 'o') continue;
    uf.snapshot();
    REP(d, 4) {
      int ni = i + dx[d];
      int nj = j + dy[d];
      if (ni < 0 or ni >= n or nj < 0 or nj >= n) continue;
      if (g[ni][nj] == 'o') {
        uf.unite(i * n + j, ni * n + nj);
      }
    }
    if (uf.size(island) == ic + 1) return true;
    uf.rollback();
  }
  return false;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "YES" : "NO");
  }
}
