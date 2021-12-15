#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Win" : "Lose") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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

template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  static_assert(std::is_invocable_r_v<bool, F, T>);
  assert(std::max(true_x, false_x) <= std::numeric_limits<T>::max() / 2);
  assert(true_x >= -1 and false_x >= -1);  // need floor_div to allow negative.

  while (std::abs(true_x - false_x) > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

template<class T>
T floor_div(T x, T y) {
  assert(y != 0);
  return x / y - (((x ^ y) < 0) and (x % y));
}
template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

auto solve() {
  int n = in, m = in, D = in;
  vector<Int> r = in(m + 1), s = in(m);

  auto f = [&](int i, int x) -> pair<Int, Int> {
    Int cnt = 0;
    Int ss = 0;
    for (int j = 1; j <= x; ++j) {
      Int qr = floor_div<Int>(r[j] - i, D) - floor_div<Int>(r[j - 1] - i, D);
      Int ql = floor_div<Int>(r[j] + i, D) - floor_div<Int>(r[j - 1] + i, D);
      Int cdelta = ql + qr;
      cnt += cdelta;
      ss += cdelta * s[j - 1];
      /*
      if (cnt >= n) {
        // no
      } else if (cdelta >= n - cnt) {
        ss += (n - cnt) * s[j - 1];
      } else {
        ss += cdelta * s[j - 1];
      }
      cnt += cdelta;
       */
    }
    return {cnt, ss};
  };

  Int ans = -1LL;
  REP(i, D) {  // i mod D
    auto mx = bisect<int>(0, m + 1, [&](int x) {
      Int qr = floor_div<Int>(r[x - 1] - i, D) - floor_div<Int>(r[x - 2] - i, D);
      Int ql = floor_div<Int>(r[x - 1] + i, D) - floor_div<Int>(r[x - 2] + i, D);
      if (ql + qr <= n) return true;
      if (qr + ql >= n + 5) {
        return false;
      }
      auto[cnt, ss] = f(i, x);
      return cnt <= n;
    });
    auto[c0, s0] = f(i, mx);
    chmax(ans, s0);
    //DUMP(i, c0, s0, ans);
    if (mx < m) {
      auto[c1, s1] = f(i, mx + 1);
      chmax(ans, s1 - (c1 - n) * s[mx]);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
