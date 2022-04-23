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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

Int solve() {
  int n = in;
  int x = in;
  vector<int> a = in(n);
  int maxa = *max_element(ALL(a));
  int mina = *min_element(ALL(a));
  Int base_score = 0;
  REP(i, 1, n) {
    base_score += abs(a[i] - a[i - 1]);
  }
  Int l_score = 0, h_score = 0;
  Int l_conn = 0, h_conn = 0;
  if (mina > 1) {
    l_conn = kBigVal<Int>;
    int minl = 1, maxl = mina - 1;
    l_score = maxl - minl;
    chmin(l_conn, abs(a.front() - minl));
    chmin(l_conn, abs(a.front() - maxl));
    chmin(l_conn, abs(a.back() - minl));
    chmin(l_conn, abs(a.back() - maxl));
    REP(i, 1, n) {
      int ha = max(a[i], a[i - 1]);
      int la = min(a[i], a[i - 1]);
      chmin(l_conn, abs(ha - maxl) + abs(la - minl) - (ha - la));
    }
  }
  if (x > maxa) {
    h_conn = kBigVal<Int>;
    int minh = maxa + 1, maxh = x;
    h_score = maxh - minh;
    chmin(h_conn, abs(a.front() - minh));
    chmin(h_conn, abs(a.front() - maxh));
    chmin(h_conn, abs(a.back() - minh));
    chmin(h_conn, abs(a.back() - maxh));
    REP(i, 1, n) {
      int ha = max(a[i], a[i - 1]);
      int la = min(a[i], a[i - 1]);
      chmin(h_conn, abs(ha - maxh) + abs(la - minh) - (ha - la));
    }
  }

  return base_score + l_score + h_score + l_conn + h_conn;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) { print(solve()); }
  exit_();
}
