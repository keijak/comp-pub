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

auto solve() -> optional<vector<int>> {
  INPUT(i64, H, W, n);
  vector<pair<int, int>> spots(n);
  vector<vector<int>> rows(H), cols(W);
  REP(i, n) {
    INPUT(int, x, y);
    --x, --y;
    spots[i] = {x, y};
    rows[x].push_back(i);
    cols[y].push_back(i);
  }

  auto par = vector(n, vector(2, -1));
  auto done = vector(n, vector(2, false));
  auto depth = vector(n, vector(2, -1));
  vector<int> anc(n, -1);

  auto dfs = [&](auto &dfs, int v, int j) -> optional<vector<int>> {
    int x, y;
    tie(x, y) = spots[v];
    vector<int> &nexts = (j & 1) ? rows[x] : cols[y];
    for (int u : nexts) {
      if (anc[u] != -1) {
        if (anc[u] == j) continue;
        if (depth[v][j] - depth[u][1 - j] < 3) continue;
        vector<int> path;
        for (int w = v, p = j; w != -1; w = par[w][p], p ^= 1) {
          path.push_back(w);
          if (w == u) break;
        }
        reverse(ALL(path));
        if (j == 0) {
          std::rotate(path.begin(), path.begin() + 1, path.end());
        }
        return path;
      }
      if (done[u][1 - j]) continue;
      done[u][1 - j] = true;
      depth[u][1 - j] = depth[v][j] + 1;
      par[u][1 - j] = v;
      anc[u] = 1 - j;
      auto sub = dfs(dfs, u, 1 - j);
      if (sub) return sub;
      anc[u] = -1;
    }
    return nullopt;
  };

  REP(i, 2) {
    REP(v, n) {
      if (done[v][i]) continue;
      done[v][i] = true;
      depth[v][i] = i;
      anc[v] = i;
      auto sub = dfs(dfs, v, i);
      if (sub) return sub;
      anc[v] = -1;
    }
  }
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print(-1);
    } else {
      auto &v = ans.value();
      print(v.size());
      for (auto &x : v) ++x;
      print_seq(v);
    }
  }
}
