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

// Note: Don't forget to update variables H and W.
template<typename T>
void rotate90(vector<vector<T>> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  auto tmp = vector(w, vector(h, T()));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[w - 1 - j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}

auto solve() {
  int n = in;
  vector<vector<char>> S(n, vector(n, '.')), T(n, vector(n, '.'));
  REP(i, n) REP(j, n) cin >> S[i][j];
  REP(i, n) REP(j, n) cin >> T[i][j];

  DUMP(S);
  DUMP(T);

  auto normalize = [&](vector<vector<char>> &g) {
    int r = -1;
    REP(i, n) {
      if (any_of(ALL(g[i]), [](char ch) { return ch == '#'; })) {
        r = i;
        break;
      }
    }
    if (r == -1) return;
    int c = -1;
    REP(j, n) {
      REP(i, n) {
        if (g[i][j] == '#') {
          c = j;
          goto exc;
        }
      }
    }
    exc:
    assert(c != -1);

    vector<vector<char>> g2(n, vector<char>(n, '.'));
    for (int i = r; i < n; ++i) {
      for (int j = c; j < n; ++j) {
        g2[i - r][j - c] = g[i][j];
      }
    }
    swap(g, g2);
  };

  normalize(S);
  REP(i, 4) {
    normalize(T);
    if (S == T) return true;
    rotate90(T);
  }

  return false;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "Yes" : "No");
  }
}
