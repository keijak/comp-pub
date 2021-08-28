#include <bits/stdc++.h>
#include <atcoder/string>

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

auto solve() -> array<int, 4> {
  int n = in;
  string s = in;
  assert(ssize(s) == n);
  for (int r = n - 1; r >= 0; --r) {
    if (r < n / 2) break;
    if (s[r] == '0') {
      DUMP(s.substr(0, r + 1), s.substr(0, r));
      return {1, r + 1, 1, r};
    }
  }
  int zacc = 0;
  for (int i = i; i < n; ++i) {
    if (n - i < n / 2) break;
    if (s[i] == '0') {
      return {i + 2, n, i + 1, n};
    }
  }
  int head = 0;
  char cur = s[0];
  for (int i = 1; i < n; ++i) {
    if (cur == s[i]) {
      int len = i - head + 1;
      if (len > n / 2) {
        return {head + 1, i, head + 2, i + 1};
      }
    } else {
      cur = s[i];
      head = i;
    }
  }
  const std::vector<int> sa = atcoder::suffix_array(s);
  const std::vector<int> lcp = atcoder::lcp_array(s, sa);
  DUMP(sa);
  DUMP(lcp);
  REP(i, n - 1) {
    if (lcp[i] >= n / 2) {
      int len = lcp[i];
      int l1 = sa[i] + 1, l2 = sa[i + 1] + 1;
      return {l1, l1 + len, l2, l2 + len};
    }
  }
  assert(false);
  return {0, 0, 0, 0};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    auto ans = solve();
    print_seq(ans);
  }
}
