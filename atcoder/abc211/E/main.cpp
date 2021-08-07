#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

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
using u64 = unsigned long long;
inline int popcount(u64 x) { return __builtin_popcountll(x); }

auto solve() {
  INPUT(int, n, K);
  vector<string> S(n);
  REP(i, n) cin >> S[i];

  int topx = 0, topy = 0;
  const array<int, 4> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
  //using P = set<pair<int, int>>;
  using P = unsigned long long;

  auto ubit = [&](int x, int y) -> P {
    int b = x * n + y;
    return P(1) << b;
  };

  auto f = [&](int x, int y) -> Int {
    P init = ubit(x, y);
    //init.insert();
    Int ans = 0;
    set<P> visited;
    queue<P> que;
    que.push(init);
    visited.insert(init);
    while (que.size()) {
      auto p = std::move(que.front());
      que.pop();
      int psz = popcount(p);
      if (psz == K) {
        ++ans;
//        auto g = S;
//        for (auto[x, y] : p) {
//          assert(g[x][y] == '.');
//          g[x][y] = '@';
//        }
//        cerr << "===" << endl;
//        REP(i, n) {
//          cerr << "| " << g[i] << endl;
//        }
      }
      if (psz >= K) continue;
      REP(j, n * n) {
        if (not(p >> j & 1)) continue;
        int x = j / n;
        int y = j % n;
        REP(i, 4) {
          int nx = x + dx[i];
          int ny = y + dy[i];
          if (nx < 0 or nx >= n or ny < 0 or ny >= n) continue;
          if (nx < topx) continue;
          if (nx == topx and ny < topy) continue;
          if (S[nx][ny] != '.') continue;
          P nubit = ubit(nx, ny);
          if (p & nubit) continue;
          P np = p | nubit;
          if (not visited.count(np)) {
            visited.insert(np);
            que.push(np);
          }
        }
      }
    }
    return ans;
  };

  Int ans = 0;
  REP(i, n) REP(j, n) {
      if (S[i][j] == '.') {
        topx = i;
        topy = j;
        ans += f(i, j);
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
    print(ans);
  }
}
