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

vector<vector<int>> solve_even(int n) {
  vector<vector<int>> groups;
  int s = n + 1;
  for (int i = n; i > (n / 2); --i) {
    groups.push_back({i, s - i});
  }
  DUMP(groups);
  return groups;
}

vector<vector<int>> solve_odd(int n) {
  vector<vector<int>> groups;
  int s = n;
  groups.push_back({n});
  for (int i = n - 1; i > (n - 1) / 2; --i) {
    groups.push_back({i, s - i});
  }
  DUMP(groups);
  return groups;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  int n = in;
  auto groups = (n % 2 == 0) ? solve_even(n) : solve_odd(n);

  vector<pair<int, int>> ans;
  REP(i, ssize(groups)) {
    for (auto x: groups[i]) {
      for (int z = 1; z < x; ++z) {
        if (find(ALL(groups[i]), z) != groups[i].end()) {
          continue;
        }
        ans.emplace_back(z, x);
      }
    }
  }

  print(ssize(ans));
  for (auto[x, y]: ans) {
    print(x, y);
  }
}
