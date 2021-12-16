#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
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

int more_crewmates(array<int, 3> a) {
  cout << "? " << a[0] << " " << a[1] << " " << a[2] << endl;
  int res;
  cin >> res;
  if (res == -1) {
    exit(0);
  }
  return res;
}

auto solve() {
  int n = in;
  bool base = more_crewmates({1, 2, 3});
  pair<int, int> gp;
  vector<int> role(n + 1);
  bool detected = false;
  for (int i = 2; i <= n - 2; ++i) {
    int res = more_crewmates({i, i + 1, i + 2});
    if (base != res) {
      gp.first = i - 1;
      gp.second = i + 2;
      role[i - 1] = base;
      role[i + 2] = res;
      detected = true;
      break;
    }
  }
  assert(detected);
  for (int i = 1; i <= n; ++i) {
    if (i == gp.first or i == gp.second) continue;
    int res = more_crewmates({gp.first, gp.second, i});
    role[i] = res;
  }
  vector<int> imposters;
  for (int i = 1; i <= n; ++i) {
    if (role[i] == 0) imposters.push_back(i);
  }
  cout << "! " << imposters.size();
  for (auto x: imposters) {
    cout << " " << x;
  }
  cout << endl;
  return Void{};
}

int main() {
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    solve();
  }
}
