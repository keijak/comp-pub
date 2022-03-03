#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

std::vector<std::pair<char, int>> runlength(const std::string &a) {
  std::vector<std::pair<char, int>> res;
  const int n = a.size();
  if (n == 0) return res;
  res.reserve(n);
  res.emplace_back(a[0], 1);
  for (int i = 1; i < n; ++i) {
    if (a[i] == res.back().first) {
      res.back().second++;
    } else {
      res.emplace_back(a[i], 1);
    }
  }
  return res;
}

auto solve() -> Int {
  string S = in;
  int n = ssize(S);
  Int K = in;
  auto rs = runlength(S);
  if (rs.size() == 1) return (K * n) / 2;

  DUMP(rs);
  if (S[0] != S[n - 1]) {
    Int z = 0;
    for (auto[ch, c]: rs) {
      z += c / 2;
    }
    return z * K;
  } else {
    Int z = 0;
    int m = ssize(rs);
    REP(i, 1, m - 1) {
      z += rs[i].second / 2;
    }
    Int ans = z * K;
    Int w = (rs[0].second + rs.back().second) / 2;
    ans += w * (K - 1);
    ans += rs[0].second / 2;
    ans += rs.back().second / 2;
    return ans;
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
