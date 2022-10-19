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

auto solve() {
  Int H = in, W = in, rs = in, cs = in;
  --rs, --cs;
  int N = in;
  map<Int, set<Int>> cols, rows;
  REP(i, N) {
    int r = in, c = in;
    --r, --c;
    cols[r].insert(c);
    rows[c].insert(r);
  }
  int Q = in;
  Int x = rs, y = cs;
  REP(i, Q) {
    char d = in;
    Int l = in;
    if (d == 'L') {
      if (not cols.count(x)) {
        y = max(y - l, 0LL);
      } else {
        auto it = cols[x].lower_bound(y);
        if (it != cols[x].begin()) {
          --it;
          y = max({y - l, *it + 1, 0LL});
        } else {
          y = max(y - l, 0LL);
        }
      }
    } else if (d == 'R') {
      if (not cols.count(x)) {
        y = min(y + l, W - 1);
      } else {
        auto it = cols[x].upper_bound(y);
        if (it != cols[x].end()) {
          y = min({y + l, *it - 1, W - 1});
        } else {
          y = min(y + l, W - 1);
        }
      }
    } else if (d == 'U') {
      if (not rows.count(y)) {
        x = max({x - l, 0LL});
      } else {
        auto it = rows[y].lower_bound(x);
        if (it != rows[y].begin()) {
          --it;
          x = max({x - l, *it + 1, 0LL});
        } else {
          x = max({x - l, 0LL});
        }
      }
    } else {
      if (not rows.count(y)) {
        x = min(x + l, H - 1);
      } else {
        auto it = rows[y].upper_bound(x);
        if (it != rows[y].end()) {
          x = min({x + l, *it - 1, H - 1});
        } else {
          x = min(x + l, H - 1);
        }
      }
    }
    out(x + 1, y + 1);
  }

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
