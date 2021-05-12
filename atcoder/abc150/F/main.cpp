#include <bits/stdc++.h>

#include <atcoder/string>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u32 = unsigned;

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

// s &= t;
// Merges two sets into their intersection.
// The first argument will be updated to the result.
template <typename T>
std::set<T> &operator&=(std::set<T> &s, const std::set<T> &t) {
  auto it = s.begin();
  auto jt = t.begin();
  while (it != s.end()) {
    while (jt != t.end() and *jt < *it) ++jt;
    if (jt == t.end()) {
      s.erase(it, s.end());
      break;
    }
    if (*it < *jt) {
      it = s.erase(it);
    } else {
      ++it, ++jt;
    }
  }
  return s;
}
template <typename T>
std::set<T> &operator&=(std::set<T> &s, std::set<T> &&t) {
  if (s.size() <= t.size()) {
    const std::set<T> &c(t);
    s &= c;
  } else {
    const std::set<T> &c(s);
    t &= c;
    s = std::move(t);
  }
  return s;
}

const u32 L = 30;

void solve() {
  int n;
  cin >> n;
  vector<u32> a(n), b(n);
  cin >> a >> b;
  set<int> ks;
  REP(k, n) ks.insert(k);
  REP(i, L) {
    vector<char> s;
    REP(j, n) { s.push_back((b[j] >> i & 1) ? '1' : '0'); }
    s.push_back('$');
    REP(p, 2) REP(j, n) { s.push_back((a[j] >> i & 1) ? '1' : '0'); }
    auto z1 = atcoder::z_algorithm(s);
    if (i == 0) {
      DUMP(s);
      DUMP(z1);
    }
    REP(j, n) { s[j] = (b[j] >> i & 1) ? '0' : '1'; }
    auto z2 = atcoder::z_algorithm(s);
    if (i == 0) {
      DUMP(z2);
    }
    REP(k, n) {
      if (z1[n + 1 + k] < n and z2[n + 1 + k] < n) ks.erase(k);
    }
  }
  if (ks.empty()) {
    cout << endl;
    return;
  }
  for (auto k : ks) {
    u32 x = b[0] ^ a[k];
    cout << k << " " << x << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
