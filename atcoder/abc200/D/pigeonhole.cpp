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

optional<pair<vector<int>, vector<int>>> solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;

  const int m = min(n, 8);

  auto restore = [&](int bits) -> vector<int> {
    vector<int> res;
    REP(i, m) {
      if (bits >> i & 1) {
        res.push_back(i);
      }
    }
    return res;
  };
  map<int, int> seen;
  REP(bits, 1, 1 << m) {
    i64 s = 0;
    REP(i, m) {
      if (bits >> i & 1) {
        s += a[i] % 200;
      }
    }
    s %= 200;
    auto it = seen.find(s);
    if (it != seen.end()) {
      auto b = restore(it->second);
      auto c = restore(bits);
      return pair(b, c);
    }
    seen[s] = bits;
  }
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (!res.has_value()) {
    cout << "No\n";
  } else {
    cout << "Yes\n";
    auto [b, c] = res.value();
    sort(ALL(b));
    sort(ALL(c));
    cout << b.size();
    for (auto x : b) cout << " " << x + 1;
    cout << endl;
    cout << c.size();
    for (auto x : c) cout << " " << x + 1;
    cout << endl;
  }
}
