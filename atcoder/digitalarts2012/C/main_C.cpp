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

using namespace std;

auto solve() {
  int n = in, m = in, K = in;
  vector<tuple<char, int, int>> logs(m);
  vector<vector<int>> tweet_time(n);
  REP(i, m) {
    auto&[c, j, k] = logs[i];
    cin >> c;
    if (c == 't') {
      cin >> j;
      --j;
      tweet_time[j].push_back(i);
    } else {
      cin >> j >> k;
      --j, --k;
    }
  }
  vector<int> twi_count(n);
  REP(i, n) {
    twi_count[i] = (int) tweet_time[i].size();
  }

  map<pair<int, int>, int> follow_time;
  REP(i, m) {
    char c;
    int j, k;
    tie(c, j, k) = logs[i];
    if (j > k) swap(j, k);
    if (c == 't') continue;
    if (c == 'f') {
      follow_time[{j, k}] = i;
    } else {
      assert(c == 'u');
      int ft = follow_time.at({j, k});
      int ut = i;
      int twi_j = lower_bound(ALL(tweet_time[j]), ut) - lower_bound(ALL(tweet_time[j]), ft);
      int twi_k = lower_bound(ALL(tweet_time[k]), ut) - lower_bound(ALL(tweet_time[k]), ft);
      twi_count[j] += twi_k;
      twi_count[k] += twi_j;
      follow_time.erase({j, k});
    }
  }
  for (auto[e, ft]: follow_time) {
    const int ut = m;
    auto[j, k] = e;
    int twi_j = lower_bound(ALL(tweet_time[j]), ut) - lower_bound(ALL(tweet_time[j]), ft);
    int twi_k = lower_bound(ALL(tweet_time[k]), ut) - lower_bound(ALL(tweet_time[k]), ft);
    twi_count[j] += twi_k;
    twi_count[k] += twi_j;
  }
  sort(ALL(twi_count), greater{});
  return twi_count[K - 1];
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
