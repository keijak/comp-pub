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
    return std::cout << (x ? "YES" : "NO") << endc;
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

auto solve() -> bool {
  const int R = in, C = in, n = in;
  vector<pair<int, int>> top, bottom, left, right;

  auto push = [&](int x, int y, int i, bool do_push) -> bool {
    if (x == 0 and y != C) {
      if (do_push) top.emplace_back(y, i);
      return true;
    }
    if (y == C and x != R) {
      if (do_push) right.emplace_back(x, i);
      return true;
    }
    if (x == R and y != 0) {
      if (do_push) bottom.emplace_back(y, i);
      return true;
    }
    if (y == 0 and x != 0) {
      if (do_push) left.emplace_back(x, i);
      return true;
    }
    return false;
  };
  REP(i, n) {
    int x1 = in, y1 = in, x2 = in, y2 = in;
    if (push(x1, y1, i, false) and push(x2, y2, i, false)) {
      push(x1, y1, i, true);
      push(x2, y2, i, true);
    }
  }
  vector<pair<int, int>> seq;
  sort(ALL(top));
  seq.insert(seq.end(), ALL(top));
  sort(ALL(right));
  seq.insert(seq.end(), ALL(right));
  sort(ALL(bottom), greater{});
  seq.insert(seq.end(), ALL(bottom));
  sort(ALL(left), greater{});
  seq.insert(seq.end(), ALL(left));
  vector<int> stk;
  for (auto [_, i]: seq) {
    if (not stk.empty() and stk.back() == i) {
      stk.pop_back();
    } else {
      stk.push_back(i);
    }
  }
  return (stk.empty());
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
