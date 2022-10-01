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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

auto solve() {
  int n = in;
  auto A = vector(n, vector(n, uint32_t{0}));
  REP(i, n) REP(j, n) cin >> A[i][j];

  auto f = [&](bool back) -> vector<unordered_map<uint32_t, Int>> {
    vector<unordered_map<uint32_t, Int>> dp(n);
    REP(bits, 1 << (n - 1)) {
      auto res = [&]() -> optional<pair<int, uint32_t>> {
        int r = back ? n - 1 : 0;
        int c = back ? n - 1 : 0;
        uint32_t val = A[r][c];
        REP(i, n - 1) {
          if (has_bit(bits, i)) {
            if (back) --r; else ++r;
            if (r < 0 or r >= n) return nullopt;
          } else {
            if (back) --c; else ++c;
            if (c < 0 or c >= n) return nullopt;
          }
          val ^= A[r][c];
        }
        if (back) val ^= A[r][c];
        return pair{r, val};
      }();
      if (not res) continue;
      auto [row, val] = *res;
      dp[row][val] += 1;
    }
    return dp;
  };
  auto dp1 = f(false);
  auto dp2 = f(true);
  DUMP(dp1);
  DUMP(dp2);
  Int ans = 0;
  REP(i, n) {
    const auto &m = dp2[i];
    for (auto [val, c]: dp1[i]) {
      auto it = m.find(val);
      if (it == m.end()) continue;
      ans += it->second * c;
    }
  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
