#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

template<typename T>
std::vector<std::pair<int, int>> min_range(const std::vector<T> &a) {
  static const T kSentinel = std::numeric_limits<T>::lowest();
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  std::deque<int> q;

  // left bound
  for (int i = n - 1; i >= -1; --i) {
    const auto &x = i >= 0 ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] >= x) {
      int j = q.back();
      res[j].first = i + 1;
      q.pop_back();
    }
    q.push_back(i);
  }
  q.clear();

  // right bound
  for (int i = 0; i <= n; ++i) {
    const auto &x = i < n ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] > x) {
      int j = q.back();
      res[j].second = i;
      q.pop_back();
    }
    q.push_back(i);
  }
  return res;
}

template<typename T>
std::vector<std::pair<int, int>> max_range(const std::vector<T> &a) {
  static const T kSentinel = std::numeric_limits<T>::max();
  const int n = a.size();
  std::vector<std::pair<int, int>> res(n);
  std::deque<int> q;

  // left bound
  for (int i = n - 1; i >= -1; --i) {
    const auto &x = i >= 0 ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] < x) {
      int j = q.back();
      res[j].first = i + 1;
      q.pop_back();
    }
    q.push_back(i);
  }
  q.clear();

  // right bound
  for (int i = 0; i <= n; ++i) {
    const auto &x = i < n ? a[i] : kSentinel;
    while (not q.empty() and a[q.back()] <= x) {
      int j = q.back();
      res[j].second = i;
      q.pop_back();
    }
    q.push_back(i);
  }
  return res;
}

using namespace std;

auto solve() {
  int n = in;
  vector<int> a = in(n);
  auto minr = min_range(a);
  auto maxr = max_range(a);
  vector dp(n + 1, Mint(0));
  vector dpsum(n + 2, Mint(0));
  vector diff(n + 2, Mint(0));
  dpsum[1] = dp[0] = 1;
  REP(i, n) {
    {
      Mint delta = (dpsum[i + 1] - dpsum[maxr[i].first]) * a[i];
      diff[i + 1] += delta;
      diff[maxr[i].second + 1] -= delta;
    }
    {
      Mint delta = (dpsum[i + 1] - dpsum[minr[i].first]) * a[i];
      diff[i + 1] -= delta;
      diff[minr[i].second + 1] += delta;
    }
    diff[i + 1] += diff[i];
    dp[i + 1] += diff[i + 1];
    dpsum[i + 2] = dpsum[i + 1] + dp[i + 1];
  }
  return dp[n];
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
