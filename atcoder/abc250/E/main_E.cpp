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
std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}
uint64_t rand_u64() {
  static std::uniform_int_distribution<uint64_t> dist(
      1000, std::numeric_limits<uint64_t>::max());
  return dist(PRNG());
}

auto solve() {
  int n = in;
  vector<uint32_t> a = in(n);
  vector<uint32_t> b = in(n);
  unordered_map < uint32_t, uint64_t > hmap;
  hmap.reserve(1 << 20);
  hmap.max_load_factor(0.25);
  for (auto x: a) {
    if (hmap.count(x)) continue;
    hmap[x] = rand_u64();
  }
  for (auto x: b) {
    if (hmap.count(x)) continue;
    hmap[x] = rand_u64();
  }
  vector<uint64_t> ha(n + 1), hb(n + 1);
  set<uint32_t> sa, sb;
  REP(i, n) {
    if (sa.count(a[i])) {
      ha[i + 1] = ha[i];
    } else {
      sa.insert(a[i]);
      ha[i + 1] = ha[i] ^ hmap[a[i]];
    }
    if (sb.count(b[i])) {
      hb[i + 1] = hb[i];
    } else {
      sb.insert(b[i]);
      hb[i + 1] = hb[i] ^ hmap[b[i]];œ
    }
  }
  int Q = in;
  REP(qi, Q) {
    int x = in, y = in;
    bool res = (ha[x] == hb[y]);
    print(res);
  }
}
œ
int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
