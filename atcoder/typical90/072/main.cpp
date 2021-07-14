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
#define VAR(type, ...) \
  type __VA_ARGS__;    \
  read_from_cin(__VA_ARGS__);

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const std::array<int, 4> dx = {1, 0, -1, 0};
const std::array<int, 4> dy = {0, 1, 0, -1};

auto solve() {
  VAR(int, H, W);
  auto C = vector(H, vector(W, '_'));
  REP(i, H) cin >> C[i];
  int si, sj;
  auto visited = vector(H, vector(W, (char)0));
  auto froms = vector(H, vector(W, (char)0));
  int ans = -1;
  auto afroms = froms;

  map<int, char> fmap;
  fmap[0] = 'v';
  fmap[1] = '>';
  fmap[2] = '^';
  fmap[3] = '<';

  auto dfs = [&](auto &rec, int i, int j, int step, int from) -> void {
    // DUMP(step, fmap[from], i, j);

    if (i == si and j == sj and step > 0) {
      if (chmax(ans, step)) {
        cerr << "# start = (" << si << ", " << sj << ") len = " << step << "\n";
        REP(p, H) {
          REP(q, W) {
            auto it = fmap.find(froms[p][q]);
            if (it != fmap.end()) {
              cerr << it->second;
            } else {
              cerr << ' ';
            }
          }
          cerr << "\n";
        }
        cerr << "\n";
      }
    }
    REP(d, 4) {
      if (d == from) continue;
      int ni = i + dx[d];
      int nj = j + dy[d];
      if (ni < 0 or ni >= H or nj < 0 or nj >= W) continue;
      if (ni < si or (ni == si and nj < sj)) continue;
      if (C[ni][nj] == '#') continue;
      if (visited[ni][nj]) continue;
      visited[ni][nj] = true;
      int fd = (d + 2) % 4;
      froms[i][j] = d;
      rec(rec, ni, nj, step + 1, fd);
      visited[ni][nj] = false;
    }
  };

  REP(i, H) REP(j, W) {
    if (C[i][j] == '#') continue;
    REP(p, H) REP(q, W) {
      visited[p][q] = 0;
      froms[p][q] = -1;
    }
    si = i, sj = j;
    dfs(dfs, i, j, 0, -1);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
