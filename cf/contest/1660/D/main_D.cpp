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
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

array<int, 2> solve() {
  int n = in;
  vector<int> a = in(n);
  vector<int> count_two(n + 1);
  vector<int> count_minus(n + 1);
  set<int> zero_pos, minus_pos;
  REP(i, n) {
    if (a[i] == 0) {
      zero_pos.insert(i);
    } else if (a[i] < 0) {
      minus_pos.insert(i);
    }
    count_two[i + 1] = count_two[i] + int(abs(a[i]) == 2);
    count_minus[i + 1] = count_minus[i] + int(a[i] < 0);
  }
  zero_pos.insert(n); // sentinel
  Int max_twos = 0;
  int ml = 0, mr = 0;
  for (int l = 0; l < n; ++l) {
    int z = *zero_pos.lower_bound(l);
    if ((count_minus[z] - count_minus[l]) % 2 == 0) {
      int twos = count_two[z] - count_two[l];
      if (twos > max_twos) {
        max_twos = twos;
        ml = l;
        mr = z;
      }
    } else {
      // odd number of minus
      auto it = minus_pos.upper_bound(z);
      assert(it != minus_pos.begin());
      --it;
      int p = *it;
      assert(p >= l);
      int twos = count_two[p] - count_two[l];
      if (twos > max_twos) {
        max_twos = twos;
        ml = l;
        mr = p;
      }
    }
  }
  DUMP(max_twos, ml, mr);
  return {ml, n - mr};
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    print_seq(solve());
  }
  exit();
}
