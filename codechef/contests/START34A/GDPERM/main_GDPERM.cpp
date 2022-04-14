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

void labo(int n) {
  vector<int> p(n);
  REP(i, n) p[i] = i + 1;
  int cnt = 0;
  do {
    if (p[n / 2] != n) continue;
    bool ok = [&]() {
      Uint val = 0;
      REP(i, n) {
        if (p[i] == i + 1) return false;
        val ^= abs(p[i] - (i + 1));
      }
      return val == 0;
    }();
    if (ok) {
      cout << "$ ";
      REP(i, n) cout << p[i] << " ";
      cout << endl;
      ++cnt;
      if (cnt == 10) return;
    }
  } while (next_permutation(ALL(p)));
  cout << "not found" << endl;
}

auto solve() {
  const int n = in;
  if (n == 1 or n == 3) {
    print(-1);
    return;
  }
  vector<int> ans(n);
  if (n == 5) {
    ans = {2, 3, 5, 1, 4};
  } else if (n % 2 == 1) {
    ans[0] = 2;
    ans[1] = 3;
    ans[2] = 1;
    ans[n - 1] = n - 1;
    ans[n - 2] = n - 2;
    ans[n - 3] = n - 3;
    ans[n - 4] = n;
    for (int i = 3; i < n - 4; i += 2) {
      ans[i] = i + 2;
      ans[i + 1] = i + 1;
    }
  } else {
    REP(i, n) {
      ans[i] = n - i;
    }
  }
  print_seq(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int T = in;
  REP(t, T) { (solve()); }
  exit_();
#if 0
  for (int i = 1; i <= 20; i += 2) {
    cout << "#" << i << endl;
    labo(i);
  }
#endif
}
