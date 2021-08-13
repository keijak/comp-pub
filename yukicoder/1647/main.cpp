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
  const int n2 = n * 2;
  vector<int> iteration(n2, -1);
  vector<int> searched(n2, -1);
  vector<int> path;

  auto dfs = [&](auto &dfs, int v, int j, int it) -> optional<int> {
    int x, y;
    tie(x, y) = spots[v];
    vector<int> &nexts = (j & 1) ? cols[y] : rows[x];
    for (int u : nexts) {
      if (u == v) continue;
      int u1 = u + ((j & 1) ? 0 : n);
      int u2 = u + ((j & 1) ? n : 0);
      if (searched[u1] != -1 and iteration[u1] == it) continue;
      if (searched[u2] != -1) {
        if (iteration[u2] != it) continue;
        int sz = ssize(path) - searched[u2];
        if (sz >= 4 and sz % 2 == 0) return searched[u2];
        continue;
      }
      searched[u2] = j - 1;
      iteration[u2] = it;
      path.push_back(u);
      auto sub = dfs(dfs, u, j + 1, it);
      if (sub.has_value()) return sub;
      path.pop_back();
    }
    return nullopt;
  };

  REP(v, n) {
    if (searched[v] != -1) continue;
    searched[v] = 0;
    iteration[v] = v;
    path.clear();
    path.push_back(v);
    auto res = dfs(dfs, v, 1, v);
    if (res.has_value()) {
      vector<int> ans;
      for (int j = res.value(); j < ssize(path); ++j) {
        ans.push_back(path[j] + 1);
      }
      if (ssize(ans) < 4) continue;
      return ans;
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
      //for (auto &x : v) ++x;
      print_seq(v);
    }
  }
}
