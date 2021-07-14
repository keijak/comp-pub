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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int K;
  cin >> K;
  string S;
  cin >> S;
  const int n = S.size();
  assert(n == (1 << K) - 1);

  vector<i64> f(n + 1, 0);

  auto update_node = [&](int x) -> void {
    int i = n - x;
    // DUMP(x, i);

    i64 rv = (x * 2 <= n ? f[x * 2] : 1);
    i64 lv = (x * 2 + 1 <= n ? f[x * 2 + 1] : 1);
    if (S[i] == '1') {
      f[x] = rv;
    } else if (S[i] == '0') {
      f[x] = lv;
    } else {
      f[x] = rv + lv;
    }
  };

  for (int x = n; x >= 1; --x) {
    update_node(x);
  }

  auto update_tree = [&](int x) -> void {
    update_node(x);
    int p = x / 2;
    for (; p >= 1; p = p / 2) {
      update_node(p);
    }
  };

  DUMP(f);
  int Q;
  cin >> Q;
  DUMP(Q);
  REP(q, Q) {
    int p;
    char c;
    cin >> p >> c;
    const int x = n - p + 1;
    // DUMP(p, c, x);
    S[p - 1] = c;
    update_tree(x);
    // DUMP(q, S, f);
    cout << f[1] << "\n";
  }
}
