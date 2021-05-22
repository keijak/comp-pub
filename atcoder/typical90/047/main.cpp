#include <bits/stdc++.h>

#include <atcoder/string>

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

int decode(char ch) {
  if (ch == 'R') return 0;
  if (ch == 'G') return 1;
  if (ch == 'B') return 2;
  assert(false);
}

i64 solve() {
  int n;
  string s, t;
  cin >> n >> s >> t;
  vector<int> sd(n - 1), td(n - 1);
  REP(i, n - 1) {
    sd[i] = (decode(s[i + 1]) - decode(s[i]) + 6) % 3;
    td[i] = (decode(t[i]) - decode(t[i + 1]) + 6) % 3;
  }
  DUMP(sd);
  DUMP(td);
  if (n == 1) {
    return 1;
  }
  i64 ans = 2;
  REP(turn, 2) {
    vector<int> v;
    if (turn == 0) {
      for (auto x : td) v.push_back(x);
      for (auto x : sd) v.push_back(x);
    } else {
      for (auto x : sd) v.push_back(x);
      for (auto x : td) v.push_back(x);
    }
    DUMP(v);

    auto z = atcoder::z_algorithm(v);
    DUMP(z);

    const int L = z.size();
    assert(L == 2 * n - 2);
    for (int i = 1; i < n - 1; ++i) {
      if (z[L - i] == i) ++ans;
    }
    if (turn == 0) {
      if (z[L - (n - 1)] == n - 1) ++ans;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
