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
void rotate90(vector<vector<T>> &grid, int &H, int &W) {
  assert((int) grid.size() == H);
  auto tmp = vector(W, vector(H, T()));
  for (int i = 0; i < H; ++i) {
    assert((int) grid[i].size() == W);
    for (int j = 0; j < W; ++j) {
      tmp[W - 1 - j][i] = grid[i][j];
    }
  }
  grid = std::move(tmp);
  std::swap(H, W);
}

template<typename T>
void sort_unique(std::vector<T> &v) {
  std::sort(v.begin(), v.end());
  v.erase(std::unique(v.begin(), v.end()), v.end());
}

auto solve() {
  int H = in, W = in, N = in;
  vector C(H, vector(W, 0));
  REP(i, H) {
    REP(j, W) {
      int c = in;
      C[i][j] = c;
    }
  }

  vector<pair<int, int>> intervals;
  auto grid = C;
  REP(trans, 2) {
    REP(i, H - 1) REP(j, W) {
        int l = grid[i][j];
        int r = grid[i + 1][j];
        if (l == r) continue;
        if (l > r) swap(l, r);
        intervals.push_back({r, l});
      }
    rotate90(grid, H, W);
  }
  sort_unique(intervals);
  DUMP(intervals);
  int last = -100;
  int cnt = 1;
  for (auto[r, l]: intervals) {
    if (l <= last) continue;
    cnt++;
    last = r - 1;
  }
  assert(cnt <= N);
  return cnt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
