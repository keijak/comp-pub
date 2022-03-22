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

// Monotonic key-value map.
struct MonotonicMap {
  // Minimum necessary `cost` to get `value`.
  map<Int, Int> value_to_cost;

  // Maximum achievable `value` by paying `cost`.
  map<Int, Int> cost_to_value;

  void add(Int cost, Int value) {
    add_minimize(value, cost);
    add_maximize(cost, value);
  }

 private:
  void add_minimize(Int value, Int cost) {
    auto it = value_to_cost.lower_bound(value);
    if (it != value_to_cost.end() and it->second <= cost) {
      return;
    }
    value_to_cost[value] = cost;
    it = value_to_cost.find(value);
    while (it != value_to_cost.begin()) {
      --it;
      if (it->second < cost) break;
      it = value_to_cost.erase(it);
    }
  }

  void add_maximize(Int cost, Int value) {
    auto it = cost_to_value.upper_bound(cost);
    if (it != cost_to_value.begin() and prev(it)->second >= value) {
      return;
    }
    cost_to_value[value] = cost;
    it = cost_to_value.find(cost);
    ++it;
    while (it != cost_to_value.end()) {
      if (it->second > value) break;
      it = cost_to_value.erase(it);
    }
  }
};

// Saturating multiplication
template<class T>
T sat_mul(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  static constexpr T kMax = std::numeric_limits<T>::max();
  static_assert(kMax != 0);
  if (T res; not __builtin_mul_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return ((x ^ y) < 0) ? kMin : kMax;
  }
}

auto solve() {
  Int n = in, C = in;
  map<Int, Int> costmap;

  auto add_cost = [&](Int hd, Int c) {
    auto it = costmap.lower_bound(hd);
    if (it != costmap.end() and it->second <= c) {
      return;
    }
    costmap[hd] = c;
    auto jt = costmap.find(hd);
    while (jt != costmap.begin()) {
      --jt;
      if (jt->second < c) break;
      jt = costmap.erase(jt);
    }
  };

  REP(i, n) {
    Int c = in, d = in, h = in;
    Int hd = h * d;
    add_cost(hd, c);
  }
  {
    vector<pair<Int, Int>> costunits;
    for (auto[hd, c]: costmap) {
      costunits.push_back({hd, c});
    }
    for (auto[hd, c]: costunits) {
      for (Int k = 2;; ++k) {
        Int kc = sat_mul(k, c);
        if (kc > C) break;
        Int khd = sat_mul(k, hd);
        if (khd >= kBigVal<Int>) break;
        DUMP(k, hd, c, khd, kc);
        add_cost(khd, kc);
      }
    }
  }
  costmap[0] = 0;
  costmap[kBigVal<Int>] = C + 1;
  DUMP(costmap);

  int m = in;
  vector<Int> ans(m);
  REP(i, m) {
    Int D = in, H = in;
    Int hd = H * D;
    auto it = costmap.lower_bound(hd + 1);
    if (it->second > C) {
      ans[i] = -1;
    } else {
      ans[i] = it->second;
    }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    print_seq(solve());
  }
}
