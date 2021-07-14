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
  vector<int> m(200, -1);
  REP(i, n) {
    int r = a[i] % 200;
    if (r == 0) {
      vector<int> b, c;
      REP(j, n) {
        b.push_back(j + 1);
        if (j != i) c.push_back(j + 1);
      }
      return pair(b, c);
    }

    if (m[r] != -1) {
      vector<int> b = {i + 1};
      vector<int> c = {m[r] + 1};
      return pair(b, c);
    } else {
      m[r] = i;
    }
  }

  vector dp(200, optional<int>());
  auto calc = [&](int val) -> vector<int> {
    vector<int> res;
    DUMP(val);
    while (dp[val].has_value()) {
      int p = dp[val].value();
      res.push_back(p + 1);
      val = (val - (a[p] % 200) + 200) % 200;
      if (val == 0) break;
    }
    return res;
  };
  REP(i, n) {
    int ri = a[i] % 200;
    if (dp[ri].has_value()) {
      auto b = calc(ri);
      vector<int> c = {i + 1};
      return pair(b, c);
    } else {
      dp[ri] = i;
    }

    for (int j = 0; j < 200; ++j) {
      if (dp[j].has_value() and dp[j].value() < i) {
        int rnew = (ri + j) % 200;
        if (dp[rnew].has_value() and dp[rnew].value() < i) {
          auto b = calc(rnew);
          auto c = calc(j);
          c.push_back(i + 1);
          return pair(b, c);
        }
        dp[rnew] = i;
      }
    }
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
    for (auto x : b) cout << " " << x;
    cout << endl;
    cout << c.size();
    for (auto x : c) cout << " " << x;
    cout << endl;
  }
}
