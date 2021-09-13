#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
  for (auto &x: a) is >> x;
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

auto solve(const int n, const string &W) {
  bool fonly = true;
  bool handx = false;
  bool firstx = false;
  Mint firsth = 0;
  Mint lastx = 0;
  Mint lasto = 0;

  Mint len = 0;
  Mint s = 0;
  Mint ans = 0;
  for (int i = 0; i < n; ++i) {
    if (not fonly) {
      if (W[i] == '.') {
        if (handx == firstx) {
          ans += ans;
          s += s + len * firsth;
          firsth += firsth;
        } else if (handx) {
          s += (s + len - lastx) + len * firsth;
          firsth += firsth + 1;
        } else {
          s += (s + len - lasto) + len * firsth;
          firsth += firsth + 1;
        }
      } else if (handx and W[i] == 'O') {
        s += len - lastx;
        firsth++;
      } else if (not handx and W[i] == 'X') {
        s += len - lasto;
        firsth++;
      }
      ans += s;
    }

    if (W[i] == '.') {
      len *= 2;
    } else if (W[i] == 'X') {
      handx = true;
      lastx = 0;
      ++lasto;
      ++len;
      if (fonly) {
        firstx = true;
        fonly = false;
      }
    } else if (W[i] == 'O') {
      handx = false;
      lasto = 0;
      ++lastx;
      ++len;
      if (fonly) {
        firstx = false;
        fonly = false;
      }
    } else {
      ++lasto;
      ++lastx;
      ++len;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve(10, "XFOF.XFOFX");

  return 0;
  const int t = in;
  REP(test_case, t) {
    const int n = in;
    const string W = in;
    assert(ssize(W) == n);
    auto ans = solve(n, W);
    cout << "Case #" << (test_case + 1) << ": ";
    print(ans);
  }
}
