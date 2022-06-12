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

inline void init_test_case(int t, int T) {
#ifdef MY_DEBUG
  if (T > 1) {
    std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
              << std::endl;
  }
#endif
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
void dump_test_case(int t, int T) {
  if (T <= 1) return;
  std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
            << std::endl;
}
#else
#define DUMP(...)
#define dump_test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

inline int countr_zero(unsigned x) {
  if (x == 0) return std::numeric_limits<unsigned>::digits;
  return __builtin_ctz(x);
}

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

void solve() {
  const int n = in;
  vector<uint32_t> a = in(n);

  int steps = 0;
  REP(i, n) {
    if (a[i] == 0) {
      ++steps;
      a[i] = 1;
    }
  }
  {
    UnionFind uf(n);
    REP(i, n) {
      REP(j, i + 1, n) {
        if (a[i] & a[j]) {
          uf.unite(i, j);
        }
      }
    }
    if (uf.size(0) == n) {
      print(steps);
      print_seq(a);
      return;
    }
  }

  REP(i, n) {
    for (int delta: {-1, +1}) {
      if (a[i] + delta <= 0) continue;
      a[i] += delta;
      UnionFind uf2(30);

      uint32_t all_bits = 0;
      REP(j, n) {
        vector<int> b;
        REP(k, 30) {
          if (has_bit(a[j], k)) {
            b.push_back(k);
            all_bits |= 1u << k;
          }
        }
        REP(p, ssize(b)) {
          REP(q, p + 1, ssize(b)) {
            uf2.unite(b[p], b[q]);
          }
        }
      }

      int r = uf2.find(countr_zero(all_bits));
      bool ok = true;
      REP(k, 30) {
        if (has_bit(all_bits, k) and uf2.find(k) != r) {
          ok = false;
          break;
        }
      }
      if (ok) {
        print(steps + 1);
        print_seq(a);
        return;
      }
      a[i] -= delta;
    }
  }

  int max_tz = -1;
  vector<int> mi;
  REP(i, n) {
    int tz = countr_zero(a[i]);
    if (tz == max_tz) {
      mi.push_back(i);
    } else if (chmax(max_tz, tz)) {
      mi.clear();
      mi.push_back(i);
    }
  }
  assert(max_tz >= 1);
  assert(not mi.empty());
  a[mi.at(0)] -= 1;
  ++steps;
  if (ssize(mi) >= 2) {
    a[mi.at(1)] += 1;
    ++steps;
  }
  print(steps);
  print_seq(a);
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    dump_test_case(t, T);
    solve();
  }
  exit_();
}
