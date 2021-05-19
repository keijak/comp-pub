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

bool solve() {
  int n, a, b, c, d;
  cin >> n >> a >> b >> c >> d;
  --a, --b, --c, --d;
  string s;
  cin >> s;

  auto move_piece = [&](int &x, const int to) -> bool {
    while (x < to) {
      if (s[x + 1] == '.') {
        ++x;
      } else if (x + 2 <= to and s[x + 2] == '.') {
        x += 2;
      } else {
        break;
      }
    }
    return x == to;
  };

  assert(a < b);
  if (c < d) {
    if (!move_piece(b, d)) return false;
    if (!move_piece(a, c)) return false;
  } else {
    bool ok = false;
    while (b <= d) {
      if (s[b - 1] == '.' and s[b + 1] == '.') {
        ok = true;
        break;
      }
      if (s[b + 1] == '.') {
        ++b;
      } else if (b + 2 <= d and s[b + 2] == '.') {
        b += 2;
      } else {
        break;
      }
    }
    if (!ok) return false;
    if (!move_piece(a, c)) return false;
    if (!move_piece(b, d)) return false;
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << (solve() ? "Yes" : "No") << "\n";
}
