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

template<class T>
struct CumSum2d {
  template<class U>
  using Vec = std::vector<U>;

  int n, m;
  Vec<Vec<T>> cum;

  template<class U>
  explicit CumSum2d(const Vec<Vec<U>> &data)
      : n(data.size()), m(data.at(0).size()), cum(n + 1, Vec<T>(m + 1, T(0))) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cum[i + 1][j + 1] =
            cum[i][j + 1] + cum[i + 1][j] - cum[i][j] + data[i][j];
      }
    }
  }

  // [x_lo, x_hi) x [y_lo, y_hi)
  T sum(int x_lo, int x_hi, int y_lo, int y_hi) const {
    assert(x_lo <= x_hi and y_lo <= y_hi);
    return cum[x_hi][y_hi] - cum[x_hi][y_lo] - cum[x_lo][y_hi] +
        cum[x_lo][y_lo];
  }
};

auto solve() {
  const int n = in, m = in;
  vector a(n, vector<Int>(m));
  vector cnt(n, vector<int>(m, 0));
  Int maxval = -kBigVal<Int>;
  REP(i, n) REP(j, m) {
      cin >> a[i][j];
      chmax(maxval, a[i][j]);
    }
  REP(i, n) REP(j, m) if (a[i][j] == maxval) cnt[i][j] = 1;
  DUMP(cnt);

  CumSum2d<int> cc(cnt);

  Int ans = n * Int(m);
  REP(h, 1, n + 1) {
    REP(w, 1, m + 1) {
      bool ok = [&]() -> bool {
        for (int x = 0; x + h <= n; ++x) {
          for (int y = 0; y + w <= m; ++y) {
            if (cc.sum(x, x + h, y, y + w) == 0) return false;
          }
        }
        return true;
      }();
      if (ok) {
        chmin(ans, h * w);
      }
    }
  }

  return ans;
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    test_case(t, T);
    output(solve());
  }
  exit_();
}
