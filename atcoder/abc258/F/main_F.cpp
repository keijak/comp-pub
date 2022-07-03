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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
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

Int dnear(Int B, Int K, Int Sx, Int Sy, Int Gx, Int Gy) {
  Int BSx = Sx / B, BSy = Sy / B;
  Int BGx = Gx / B, BGy = Gy / B;
  Int ans = (abs(Sx - Gx) + abs(Sy - Gy)) * K;  // baseline
  if (BSx == BGx and BSy == BGy) {
    chmin(ans, (abs(Sx - BSx * B) + abs(Gx - BSx * B)) * K + abs(Sy - Gy));
    chmin(ans, (abs(Sx - (BSx + 1) * B) + abs(Gx - (BSx + 1) * B)) * K + abs(Sy - Gy));
    chmin(ans, (abs(Sy - BSy * B) + abs(Gy - BSy * B)) * K + abs(Sx - Gx));
    chmin(ans, (abs(Sy - (BSy + 1) * B) + abs(Gy - (BSy + 1) * B)) * K + abs(Sx - Gx));
  }
  if (abs(BSx - BGx) == 1) {
    Int b = min(BSx, BGx) + 1;
    chmin(ans, (abs(Sx - b * B) + abs(Gx - b * B)) * K + abs(Sy - Gy));
  }
  if (abs(BSy - BGy) == 1) {
    Int b = min(BSy, BGy) + 1;
    chmin(ans, (abs(Sy - b * B) + abs(Gy - b * B)) * K + abs(Sx - Gx));
  }
  return ans;
}

auto solve() -> Int {
  const Int B = in, K = in;
  Int Sx = in, Sy = in, Gx = in, Gy = in;
  Int BSx = Sx / B, BSy = Sy / B;
  Int BGx = Gx / B, BGy = Gy / B;
  Int ans = dnear(B, K, Sx, Sy, Gx, Gy);
  for (auto [ax, ay]: {pair{0, 0}, pair{0, 1}, pair{1, 0}, pair{1, 1}}) {
    Int sx = (BSx + ax) * B, sy = (BSy + ay) * B;
    Int da = dnear(B, K, Sx, Sy, sx, sy);
    for (auto [bx, by]: {pair{0, 0}, pair{0, 1}, pair{1, 0}, pair{1, 1}}) {
      Int gx = (BGx + bx) * B, gy = (BGy + by) * B;
      Int db = dnear(B, K, Gx, Gy, gx, gy);
      chmin(ans, abs(sx - gx) + abs(sy - gy) + da + db);
    }
  }
  return ans;
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
