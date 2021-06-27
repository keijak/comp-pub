#include <atcoder/maxflow>
#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...)                                             \
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
inline int ssize(const T &a) { return (int) std::size(a); }

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a)
    is >> x;
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
    if (it != b)
      os << sep;
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
void print(const T &x) { std::cout << x << "\n"; }
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
#define INPUT(type, ...)                                                       \
  type __VA_ARGS__;                                                            \
  read_from_cin(__VA_ARGS__);

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const std::array<int, 8> dx = {1, 1, 0, -1, -1, -1, 0, +1};
const std::array<int, 8> dy = {0, 1, 1, +1, 0, -1, -1, -1};

auto solve() -> optional<vector<int>> {
  INPUT(int, n, T);
  vector<array<i64, 2>> a(n), b(n);
  REP(i, n) cin >> a[i][0] >> a[i][1];
  map<array<i64, 2>, int> mb;
  REP(i, n) {
    cin >> b[i][0] >> b[i][1];
    mb[b[i]] = i;
  }

  atcoder::mf_graph<int> g(n * 2 + 2);
  const int kSource = n * 2;
  const int kSink = n * 2 + 1;

  vector<tuple<int, int, int>> edges;
  REP(i, n) {
    auto[x, y] = a[i];
    bool has_b = false;
    REP(d, 8) {
      i64 nx = x + dx[d] * T;
      i64 ny = y + dy[d] * T;
      auto it = mb.find({nx, ny});
      if (it == mb.end())
        continue;
      has_b = true;
      const int j = it->second;
      g.add_edge(i, n + j, 1);
      edges.emplace_back(i, j, d);
    }
    if (not has_b) return nullopt;
  }
  REP(i, n) {
    g.add_edge(kSource, i, 1);
    g.add_edge(n + i, kSink, 1);
  }
  int res = g.flow(kSource, kSink);
  if (res != n) return nullopt;
  vector<int> ans(n, -1);
  REP(i, edges.size()) {
    auto[from, to, dir] = edges[i];
    const auto &e = g.get_edge(i);
    assert(e.from == from);
    assert(e.to == n + to);
    if (e.flow != 0) {
      ans[from] = dir + 1;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print("No");
    } else {
      const auto &v = ans.value();
      for (auto d : v) {
        assert(d >= 0);
      }
      print("Yes");
      print_seq(v);
    }
  }
}
