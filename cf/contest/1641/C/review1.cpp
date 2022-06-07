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

auto solve() {
  const int n = in, Q = in;
  map<int, int> rmap;
  REP(i, n) {
    rmap[i] = kBigVal<int>;
  }

  REP(qi, Q) {
    const int t = in;
    if (t == 0) {
      int l = in, r = in, x = in;
      --l;
      if (x == 0) {
        auto rit = rmap.lower_bound(r);
        int minr = kBigVal<int>;
        for (auto it = rmap.lower_bound(l); it != rit;) {
          chmin(minr, it->second);
          it = rmap.erase(it);
        }
        if (rit != rmap.end()) {
          assert(minr >= rit->first);
          chmin(rit->second, minr);
        }
      } else {
        auto it = rmap.lower_bound(l);
        assert(it != rmap.end());
        auto rit = rmap.lower_bound(r);
        assert(rit != rmap.begin());
        --rit;
        chmin(it->second, rit->first);
      }
    } else {
      const int j = int(in) - 1;
      auto it = rmap.find(j);
      if (it == rmap.end()) {
        print("NO");
      } else {
        auto nit = std::next(it);
        if (it->second < kBigVal<int> and (nit == rmap.end() or nit->first > it->second)) {
          print("YES");
        } else {
          print("N/A");
        }
      }
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}