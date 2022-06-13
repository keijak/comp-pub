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

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));  // No input is left behind.
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
void test_case(int t, int T) {
  if (T <= 1) return;
  std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
            << std::endl;
}
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define test_case(...)
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

char ask1(int i) {
  cout << "? 1 " << (i + 1) << endl;
  char c;
  cin >> c;
  return c;
}

// number of distinct chars in [l, r)
int ask2(int l, int r) {
  cout << "? 2 " << (l + 1) << " " << r << endl;
  int a;
  cin >> a;
  return a;
}

// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > T(1)) {
    auto mid = (truthy & falsy) + (truthy ^ falsy) / T(2);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

auto solve() {
  const int n = in;
  string S(n, '#');
  S[0] = ask1(0);
  vector<int> last(26, -1);
  last[S[0] - 'a'] = 0;
  int nd = 1;
  REP(i, 1, n) {
    DUMP(S);
    int m = ask2(0, i + 1);
    if (m == nd + 1) {
      S[i] = ask1(i);
      last[S[i] - 'a'] = i;
      ++nd;
      continue;
    }
    assert(m == nd);
    vector<int> li;
    REP(j, 26) if (last[j] != -1) li.push_back(last[j]);
    sort(ALL(li), greater{});
    DUMP(li);
    int x = bisect(ssize(li), -1, [&](int j) {
      int a = (li[j] == 0) ? m : ask2(li[j], i + 1);
      DUMP(j, li[j], a);
      return a == j + 1;
    });
    DUMP(x, S[li[x]]);
    S[i] = S[li[x]];
    last[S[i] - 'a'] = i;
  }
  cout << "! " << S << endl;
}

int main() {
  init_(true);
  solve();
  exit_();
}
