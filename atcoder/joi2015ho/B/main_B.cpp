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

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

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

auto solve() -> Int {
  int n = in;
  vector<Int> a = in(n);
  if (n == 1) {
    return a[0];
  }
  vector<Int> aa;
  aa.reserve(3 * n);
  REP(j, 3) REP(i, n) aa.push_back(a[i]);

  const int L = n;
  auto dpf = vector(L, vector(L, -1LL));
  auto dpg = vector(L, vector(L, -1LL));
  auto f = [&](auto &f, auto &g, int l, int r) -> Int {
    if (r - l == 0) return 0;
    if (r - l == 1) return aa[l];
    int lm = l % n, rm = r % n;
    if (dpf[lm][rm] != -1) return dpf[lm][rm];
    auto ret = std::max(
        g(g, f, l + 1, r) + aa[l],
        g(g, f, l, r - 1) + aa[r - 1]
    );
    dpf[lm][rm] = ret;
    return ret;
  };
  auto g = [&](auto &g, auto &f, int l, int r) -> Int {
    if (r - l <= 1) return 0;
    int lm = l % n, rm = r % n;
    if (dpg[lm][rm] != -1) return dpg[lm][rm];
    Int ret;
    if (aa[l] > aa[r - 1]) {
      ret = f(f, g, l + 1, r);
    } else {
      ret = f(f, g, l, r - 1);
    }
    dpg[lm][rm] = ret;
    return ret;
  };

  Int ans = -kBigVal<Int>;
  REP(i, 1, n + 1) {
    Int ret;
    if (aa[i - 1] > aa[i + 1]) {
      ret = f(f, g, i + 1, i + 1 + (n - 2)) + aa[i];
    } else {
      ret = f(f, g, i + 2, i + 2 + (n - 2)) + aa[i];
    }
    chmax(ans, ret);
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
