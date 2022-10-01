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

auto solve() -> optional<string> {
  int n = in, S = in;
  vector<Int> D(n);
  vector<char> smallb(n);
  Int s0 = 0;
  REP(i, n) {
    Int a = in, b = in;
    if (a > b) {
      swap(a, b);
      smallb[i] = true;
    }
    s0 += a;
    D[i] = b - a;
  }
  DUMP(s0, D);
  if (s0 > S) {
    return nullopt;
  }
  const int W = S - s0;
  auto dp = vector(n + 1, vector(W + 1, 0));
  dp[0][0] = 1;
  REP(i, n) {
    dp[i + 1] = dp[i];
    int delta = D[i];
    REP(j, W + 1) {
      if (dp[i][j] and j + delta <= W) {
        dp[i + 1][j + delta] = true;
      }
    }
    DUMP(i + 1, dp[i + 1]);
  }
  DUMP(dp[n][W]);
  if (dp[n][W] == 0) return nullopt;
  deque<int> ix;
  {
    int i = n, j = W;
    while (i >= 0) {
      assert(dp[i][j]);
      if (i == 0 and j == 0) break;
      if (dp[i - 1][j]) {
        --i;
        continue;
      }
      int delta = D[i - 1];
      assert(j - delta >= 0);
      assert(dp[i - 1][j - delta]);
      ix.push_front(i - 1);
      j -= delta;
      i -= 1;
    }
    DUMP(ix);
  }
  vector<int> res(n);
  REP(i, n) {
    res[i] = smallb[i] ? 1 : 0;
  }
  for (int j: ix) {
    res[j] ^= 1;
  }
  string ans(n, '_');
  REP(i, n) {
    ans[i] = res[i] ? 'T' : 'H';
  }
  return ans;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    auto ans = (solve());
    if (not ans) {
      out("No");
    } else {
      out("Yes");
      out(*ans);
    }
  }
  exit_();
}
