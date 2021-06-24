#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <array>
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
  read_from_cin(__VA_ARGS__);

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const std::array<int, 4> dx = {-2, -2, +1, -1};
const std::array<int, 4> dy = {+1, -1, -2, -2};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int N = 15;

  auto dp = vector(N, vector(N, -1));
  dp[0][0] = 0;
  for (int s = 0; s <= (N - 1) * 2; ++s) {
    for (int x = 0; x < N; ++x) {
      int y = s - x;
      if (y < 0) break;
      if (y >= N) continue;
      set<int> nb;
      REP(d, 4) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx < 0 or nx >= N or ny < 0 or ny >= N) {
          continue;
        }
        nb.insert(dp[nx][ny]);
      }
      int mex = 0;
      for (auto z : nb) {
        if (z > mex) break;
        if (z == mex) ++mex;
      }
      dp[x][y] = mex;
    }
  }

  int t = 1;
  cin >> t;
  REP(test_case, t) {
    INPUT(int, x, y);
    --x, --y;
    auto nimber = dp[x][y];
    print(nimber != 0 ? "First" : "Second");
  }
}
