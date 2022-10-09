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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

// Generates values in [lo, hi].
int rand_int(int lo, int hi) {
  std::uniform_int_distribution<int> rand(lo, hi);
  return rand(PRNG());
}

constexpr int MX = 1'000'000'000;

// Returns all divisors of n. O(sqrt(n)) + sorting.
std::vector<Int> divisors(Int n) {
  std::vector<Int> res;
  for (Int k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    Int q = n / k;
    if (q != k) res.push_back(q);
  }
  //std::sort(res.begin(), res.end());
  return res;
}

auto solve() -> optional<int> {
  int n = in;
  vector<int> a = in(n);
  map<int, int> freq;
  for (int x: a) freq[x] += 1;
  int half = n / 2;
  for (auto [x, c]: freq) {
    if (c > half) {
      return MX;
    }
  }

  unordered_set < Int > checked;
  auto check = [&](Int z) -> bool {
    if (checked.count(z)) return false;
    checked.insert(z);
    map<int, int> fq;
    for (int x: a) {
      fq[x % z] += 1;
    }
    for (auto [x, c]: fq) {
      if (c > half) {
        return true;
      }
    }
    return false;
  };

  REP(trial, 50) {
    Int x, y;
    bool ok = false;
    REP(trial2, 100) {
      x = a[rand_int(0, n - 1)];
      y = a[rand_int(0, n - 1)];
      if (abs(x - y) <= 2) continue;
      ok = true;
      break;
    }
    if (not ok) continue;
    Int d = abs(x - y);
    auto divs = divisors(d);
    for (Int z: divs) {
      if (z <= 2) continue;
      if (check(z)) return z;
    }
  }
  return nullopt;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    auto res = solve();
    out(res.value_or(-1));
  }
  exit_();
}
