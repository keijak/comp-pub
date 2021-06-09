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

const i64 BIG = 1e15;

auto solve() {
  int a, b;
  cin >> a >> b;
  vector<int> A(a), B(b);
  cin >> A >> B;

  auto dp = vector(a + b, vector(a + 1, optional<i64>()));

  auto f = [&](auto &f, int i, int pa) -> i64 {
    if (i >= a + b) return 0;
    if (dp[i][pa].has_value()) return *dp[i][pa];
    int pb = i - pa;
    i64 res;

    if (i % 2 == 0) {
      // first
      res = -BIG;
      if (pa < a) {
        chmax(res, A[pa] + f(f, i + 1, pa + 1));
      }
      if (pb < b) {
        chmax(res, B[pb] + f(f, i + 1, pa));
      }
    } else {
      // second
      res = BIG;
      if (pa < a) {
        chmin(res, f(f, i + 1, pa + 1));
      }
      if (pb < b) {
        chmin(res, f(f, i + 1, pa));
      }
    }
    dp[i][pa] = res;
    return res;
  };
  return f(f, 0, 0);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
