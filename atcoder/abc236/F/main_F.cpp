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
using u32 = uint32_t;

auto solve() {
  int n = in;
  int m = (1 << n) - 1;
  vector<Int> C = in(m);
  vector<pair<Int, u32>> spice(m);
  REP(i, m) {
    spice[i] = {C[i], i + 1};
  }
  sort(ALL(spice));

  // basis[i] = {y, k} means:
  // - i-th bit を a[k] が担当する
  // - y の highest 1-bit が (1 << i)
  // - a[0] 〜 a[k] を使って y が作れる
  vector<optional<pair<u32, int>>> basis(n);
  Int mincost = 0;
  int okcnt = 0;
  REP(i, m) {
    auto[c, x] = spice[i];
    bool ok = false;
    for (int j = n - 1; j >= 0; --j) {
      if (not(x >> j & 1)) continue;
      if (basis[j].has_value()) {
        auto[y, k] = basis[j].value();
        x ^= y;
      } else {
        basis[j] = pair(x, i);
        ok = true;
        break;
      }
    }
    if (ok) {
      ++okcnt;
      mincost += c;
    }
  }
  DUMP(n, okcnt, mincost);
  assert(okcnt == n);
  return mincost;
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
