#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::optional<T> &a) {
  if (a.has_value()) {
    return os << "Some(" << a.value() << ")";
  } else {
    return os << "None";
  }
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
const int B = 61;

auto solve() {
  int n;
  cin >> n;
  vector<u64> a(n);
  cin >> a;
  sort(ALL(a), greater{});

  // REP(i, n) {
  //   bitset<64> b(a[i]);
  //   cerr << i << "\t" << b << " " << a[i] << endl;
  // }

  u64 vall = 0;
  REP(i, n) { vall ^= a[i]; }
  // {
  //   bitset<64> b(vall);
  //   cerr << "ALL\t" << b << " " << vall << endl;
  // }

  vector<optional<pair<u64, int>>> basis(B);
  REP(i, n) {
    u64 x = a[i];
    for (int j = B - 1; j >= 0; --j) {
      if (vall >> j & 1) continue;
      if (not(x >> j & 1)) continue;
      if (basis[j].has_value()) {
        auto [y, k] = basis[j].value();
        x ^= y;
      } else {
        basis[j] = pair(x, i);
        break;
      }
    }
  }
  DUMP(basis);

  u64 v1 = 0;
  for (int i = B - 1; i >= 0; --i) {
    if (vall >> i & 1) continue;
    if (v1 >> i & 1) continue;
    if (not basis[i].has_value()) continue;
    auto [y, j] = basis[i].value();
    v1 ^= y;
  }
  u64 v2 = vall ^ v1;
  DUMP(v1, v2);
  u64 ans = v1 + v2;
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
