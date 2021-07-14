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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DEBUG(...)
#endif

using namespace std;

int n;
int qcount;

int query(int t, int i, int j, int x) {
  ++qcount;
  if (qcount > (3 * n) / 2 + 30) {
    DEBUG("Exceeds the query limit.");
    assert(false);
  }
  cout << "? " << t << " " << i + 1 << " " << j + 1 << " " << x << endl;
  int res;
  cin >> res;
  return res;
}

void solve() {
  cin >> n;
  vector p(n, -1);
  int one_index = -1;
  qcount = 0;
  for (int i = 0; i < n; i += 2) {
    int x;
    if (i == n - 1) {
      one_index = i;
      break;
    }
    x = query(2, i, i + 1, 1);
    if (x >= 3) continue;
    if (x == 1) {
      one_index = i;
      break;
    }
    if (x == 2) {
      x = query(2, i + 1, i, 1);
      if (x == 1) {
        one_index = i + 1;
        break;
      }
    }
  }
  assert(one_index >= 0);
  p[one_index] = 1;

  for (int i = 0; i < n; ++i) {
    if (i == one_index) continue;
    p[i] = query(1, one_index, i, n - 1);
  }
  cout << "!";
  REP(i, n) {
    assert(p[i] > 0);
    assert(p[i] <= n);
    cout << " " << p[i];
  }
  cout << endl;
}

int main() {
  int t;
  cin >> t;
  REP(i, t) { solve(); }
}
