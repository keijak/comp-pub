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
  for (auto &x: a) is >> x;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename T>
void transpose(vector<vector<T>> &grid) {
  int n = grid.size();
  if (n == 0) return;
  int m = grid[0].size();
  if (m == 0) return;
  auto tmp = vector(m, vector(n, grid[0][0]));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      tmp[j][i] = grid[i][j];
    }
  }
  grid = std::move(tmp);
}

auto solve() -> optional<pair<int, int>> {
  const int n = in;
  auto C = vector(n, vector<char>(n, '.'));
  REP(i, n) {
    string row = in;
    assert(ssize(row) == n);
    REP(j, n) C[i][j] = row[j];
    DUMP(i, C[i]);
  }
  int min_moves = 1000;
  int ways = 0;

  set<pair<int, int>> one_set;

  REP(rot, 2) {
    REP(i, n) {
      int co = std::count(ALL(C[i]), 'O');
      if (co) continue;
      int ce = std::count(ALL(C[i]), '.');
      if (ce > min_moves) continue;
      if (ce < min_moves) {
        min_moves = ce;
        ways = 1;
      } else {
        ++ways;
      }
      if (ce == 1) {
        int j = find(ALL(C[i]), '.') - C[i].begin();
        assert(j < n and C[i][j] == '.');
        if (rot) {
          one_set.insert({j, i});
        } else {
          one_set.insert({i, j});
        }
      }
    }
    transpose(C);
  }
  if (min_moves == 1000) return nullopt;
  if (min_moves == 1) {
    DUMP(one_set);
    return pair{1, ssize(one_set)};
  }
  return pair{min_moves, ways};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    DUMP(test_case);
    auto ans = solve();
    cout << "Case #" << (test_case + 1) << ": ";
    if (not ans) {
      print("Impossible");
    } else {
      auto[a, b] = ans.value();
      print(a, b);
    }
  }
}