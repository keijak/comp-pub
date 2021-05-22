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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const int BIG = 1e9;

map<tuple<int, int, int, bool>, int> memo;

// Maximizes: (opponent's cost) - (my cost).
int f(int a, int b, int c, bool norev) {
  if (a == 0 and b == 0 and c == 0) {
    return 0;
  }
  tuple<int, int, int, bool> mkey = {a, b, c, norev};
  if (auto it = memo.find(mkey); it != memo.end()) {
    return it->second;
  }
  int ans = -BIG;
  if (a > 0) {
    chmax(ans, -f(a - 1, b, c, false) - 1);
  }
  if (b > 0) {
    chmax(ans, -f(a + 1, b - 2, c, false) - 1);
  }
  if (c > 0) {
    chmax(ans, -f(a, b, c - 1, false) - 1);
  }
  if (!norev and a > 0) {
    chmax(ans, -f(a, b, c, true));
  }
  assert(ans != -BIG);
  memo[mkey] = ans;
  return ans;
}

int solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  int a = 0, b = 0, c = 0;
  REP(i, n) {
    if (s[i] == '0') {
      if (n % 2 == 1 and i == n / 2) {
        ++c;  // center
      } else if (s[n - 1 - i] == '1') {
        ++a;
      } else {
        ++b;
      }
    }
  }
  DUMP(a, b, c);
  return f(a, b, c, false);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DUMP("test case", i);
    int res = solve();
    if (res > 0) {
      cout << "ALICE\n";
    } else if (res < 0) {
      cout << "BOB\n";
    } else {
      cout << "DRAW\n";
    }
  }
}
