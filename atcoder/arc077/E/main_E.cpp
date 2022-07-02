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

auto solve() {
  const int n = in, m = in;
  vector<Int> a = in(n);
  Int total = 0;
  REP(i, 1, n) {
    total += (a[i] - a[i - 1] + m) % m;
  }
  vector<Int> D(m + 10);
  REP(i, 1, n) {
    if (a[i] == a[i - 1]) continue;
    if (a[i] == a[i - 1] + 1) continue;
    if (a[i] >= a[i - 1] + 2) {
      Int head = a[i - 1] + 2;
      Int tail = a[i] + 1;
      Int w = tail - head;
      D[head] += 1;
      D[tail] -= w + 1;
      D[tail + 1] += w;
    } else {
      Int head = a[i - 1] + 2;
      Int tail = m + 1;
      Int w = tail - head;
      D[head] += 1;
      D[tail] -= w + 1;
      D[tail + 1] += w;
      {
        Int head2 = 1;
        Int tail2 = a[i] + 1;
        Int w2 = w + tail2 - head2;
        D[1] += w + 1;
        D[2] -= w;
        D[tail2] -= w2 + 1;
        D[tail2 + 1] += w2;
      }
    }
  }
  DUMP(D);
  REP(j, 2) {
    REP(i, 1, m + 10) {
      D[i] += D[i - 1];
    }
    DUMP(D);
  }
  Int dmax = *max_element(ALL(D));
  DUMP(total, dmax);
  return total - dmax;
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
