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

// Z-algorithm
// Returns the Z array of the given string.
// Z[i] := the longest common prefix length between S[0:] and S[i:].
std::vector<int> ZArray(std::string_view s) {
  const int n = s.size();
  std::vector<int> Z(n);
  Z[0] = n;
  int i = 1, j = 0;
  while (i < n) {
    while (i + j < n and s[j] == s[i + j]) ++j;
    Z[i] = j;
    if (j == 0) {
      ++i;
      continue;
    }
    int k = 1;
    for (; k < j and k + Z[k] < j; ++k) {
      Z[i + k] = Z[k];
    }
    i += k;
    j -= k;
  }
  return Z;
}

string solve() {
  int n, k;
  cin >> n >> k;
  string s;
  cin >> s;
  assert(ssize(s) == n);
  string s2 = s + s + s;
  auto z = ZArray(s2);
  int l = 1;
  for (int i = 1; i <= n;) {
    if (i == n) {
      l = n;
      break;
    }
    if (s2[i] > s2[0]) {
      l = i;
      break;
    }
    if (s2[i] < s2[0]) {
      ++i;
      continue;
    }
    int k = z[i];
    if (k < n and s2[i + k] < s2[k]) {
      i = i + k + 1;
      continue;
    }
    l = i;
    break;
  }
  int tot = 0;
  vector<char> ans;
  ans.reserve(k);
  while (tot < k) {
    for (int i = 0; i < l and tot < k; ++i) {
      ans.push_back(s2[i]);
      tot++;
    }
  }
  return string(ALL(ans));
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
