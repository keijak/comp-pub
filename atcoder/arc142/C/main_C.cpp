// #define NDEBUG
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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &output_seq(
    const Container &seq, const char *sep = " ", const char *ends = "\n",
    std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &output_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &output(const T &x) {
  return output_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &output(const T &head, Ts... tail) {
  return output_one(head, ' '), output(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

int query(int u, int v) {
  cout << "? " << u << " " << v << endl;
  int d;
  cin >> d;
  return d;
}

auto solve() {
  int n;
  cin >> n;
  vector<vector<int>> ds(n);
  int max_delta = -1;
  int min_sum = kBigVal<int>;
  bool has_same_dist = false;
  vector<int> v21, v12;
  for (int v = 3; v <= n; ++v) {
    for (int u = 1; u <= 2; ++u) {
      int d = query(u, v);
      ds[v - 1].push_back(d);
    }
    int x = ds[v - 1][0];
    int y = ds[v - 1][1];
    chmax(max_delta, abs(x - y));
    chmin(min_sum, x + y);
    if (x == y) has_same_dist = true;
    if (x == 1 and y == 2) v12.push_back(v);
    if (x == 2 and y == 1) v21.push_back(v);
  }
  if (n == 3) {
    return (ds[2][0] == ds[2][1]) ? 2 : 1;
  }
  if (max_delta >= 2 or has_same_dist) {
    return min_sum;
  }
  if (n == 4 and v12.size() == 1 and v21.size() == 1) {
    int d = query(v12.front(), v21.front());
    return d == 1 ? 3 : 1;
  }
  return 1;
}

int main() {
  init_(true);
  int ans = solve();
  cout << "! " << ans << endl;
  exit_();
}
