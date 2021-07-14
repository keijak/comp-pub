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

string solve() {
  string A;
  cin >> A;
  const int n = ssize(A);

  deque<int> heads;
  deque<vector<int>> index;
  auto B = vector(n + 1, vector(26, -1));
  int alphabet_count = 0;
  for (int i = n - 1; i >= 0; --i) {
    int ch = A[i] - 'a';
    B[i] = B[i + 1];
    if (B[i][ch] == -1) {
      ++alphabet_count;
    }
    B[i][ch] = i;
    if (alphabet_count == 26) {
      heads.push_front(i);
      index.push_front(B[i]);
      B[i].assign(26, -1);
      alphabet_count = 0;
    }
  }

  DUMP(heads);
  //   REP(i, n) { DUMP(i, B[i]); }

  const int m = ssize(heads) + 1;
  string ans(m, '_');
  int p = 0;
  REP(i, m) {
    char ch = '?';
    REP(j, 26) {
      if (B[p][j] == -1) {
        ch = 'a' + j;
        if (i < ssize(heads)) {
          p = index[i][j] + 1;
        }
        break;
      }
    }
    assert(ch != '?');
    ans[i] = ch;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
