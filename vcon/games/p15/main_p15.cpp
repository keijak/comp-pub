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
    return std::cout << (x ? "first" : "second") << endc;
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

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

bool labo(int n, int K) {
  Uint full = (1ull << n) - 1;
  map<pair<Uint, int>, bool> memo;

  auto win = Rec([&](auto &self, Uint s, int last) -> bool {
    if (s == 0) return false;
    if (auto it = memo.find({s, last}); it != end(memo)) return it->second;
    bool ret = false;
    REP(i, n) {
      if (not has_bit(s, i)) continue;
      if (abs(i - last) <= K) continue;
      if (not self(s & ~(1ull << i), i)) {
        ret = true;
        break;
      }
    }
    memo[{s, last}] = ret;
    return ret;
  });

  bool ok = false;
  REP(i, n) {
    if (not win(full & ~(1ull << i), i)) {
      ok = true;
      break;
    }
  }
  return ok;
}

bool solve(int n, int K) {
  if (n & 1) return true;
  return K >= n / 2;
}

int main() {

#if 0
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, K = in;
  print(solve(n, K));
#else
  for (int n = 1; n <= 50; ++n) {
    printf("n=%2d|", n);
    for (int k = 0; k <= n; ++k) {
      int ans = labo(n, k);
      cout << ans << " ";
    }
    cout << endl;
  }

#endif
}
