#include <atcoder/fenwicktree>
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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

int solve() {
  const string S = in;
  const int n = ssize(S);
  vector<int> scount(n), sindex;
  REP(i, n) {
    scount[i] = ssize(sindex);
    if (S[i] == 'Y') {
      sindex.push_back(i);
    }
  }
  const int total_y = ssize(sindex);
  DUMP(total_y, sindex);
  if (total_y == 0) {
    return 0;
  }

  vector<Int> lcounts(total_y), lsums(total_y);
  REP(i, 1, total_y) {
    Int gap = sindex[i] - sindex[i - 1] - 1;
    lcounts[i] = gap;
    lcounts[i] += lcounts[i - 1];
    lsums[i] = gap * i;
    lsums[i] += lsums[i - 1];
  }
  vector<Int> rcounts(total_y), rsums(total_y);
  for (int i = total_y - 2; i >= 0; --i) {
    Int gap = sindex[i + 1] - sindex[i] - 1;
    rcounts[i] = gap;
    rcounts[i] += rcounts[i + 1];
    rsums[i] = gap * (total_y - 1 - i);
    rsums[i] += rsums[i + 1];
  }

  const Int K = in;

  auto is_good = [&](int l, int r) -> bool {
    const int m = l + (r - l) / 2;
    Int cost = 0;
    cost += (lsums[m] - lsums[l]);
    cost -= (lcounts[m] - lcounts[l]) * l;
    cost += (rsums[m] - rsums[r - 1]);
    cost -= (rcounts[m] - rcounts[r - 1]) * (total_y - r);
    return (cost <= K);
  };

  int ans = 0;
  int l = 0;
  for (int r = 1; r <= total_y; ++r) {
    while (l < r and not is_good(l, r)) ++l;
    if (l < r) {
      chmax(ans, r - l);
    }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { print(solve()); }
  exit_();
}
