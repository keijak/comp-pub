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

template<typename T, typename U = std::make_unsigned_t<T>>
int popcount(T x) {
  if constexpr (std::is_same_v<U, unsigned>) {
    return __builtin_popcount(static_cast<U>(x));
  } else if constexpr (std::is_same_v<U, unsigned long>) {
    return __builtin_popcountl(static_cast<U>(x));
  } else if constexpr (std::is_same_v<U, unsigned long long>) {
    return __builtin_popcountll(static_cast<U>(x));
  }
  assert(false);  // unsupported type
}

inline int msb_log(unsigned x) {
  assert(x != 0);
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
}
inline int msb_log(Uint x) {
  assert(x != 0);
  return std::numeric_limits<Uint>::digits - __builtin_clzll(x) - 1;
}

inline int bit_width(unsigned x) {
  if (x == 0) return 0;
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x);
}
inline int bit_width(Uint x) {
  if (x == 0) return 0;
  return std::numeric_limits<Uint>::digits - __builtin_clzll(x);
}
template<typename T, typename U = std::make_unsigned_t<T>>
inline U bit_floor(T x) {
  if (x <= 2) return x;
  return U(1) << (bit_width(U(x)) - 1);
}

int gray_code(int n) { return n ^ (n >> 1); }

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

using namespace std;

bool solve() {
  const int N = in, K = in;
  vector<pair<unsigned, unsigned>> basis;
  for (uint32_t bits = (1 << N) - 1; bits > 0; --bits) {
    if (ssize(basis) == N) break;
    if (popcount(bits) != K) continue;
    uint32_t v = bits;
    for (auto [e, _]: basis) {
      v = min(v, v ^ e);
    }
    if (v > 0) basis.emplace_back(v, bits);
  }

  if (ssize(basis) < N) {
    return false;
  }

  vector<uint32_t> ans(1 << N);
  REP(i, 1 << N) {
    uint32_t val = 0;
    uint32_t g = gray_code(i);
    REP(b, N) {
      if (has_bit(g, b)) {
        uint32_t kb = basis[b].second;
        val ^= kb;
      }
    }
    ans[i] = val;
  }
  for (int i = 0; i < (1 << N); ++i) {
    int j = (i + 1) % (1 << N);
    int db = popcount(ans[i] ^ ans[j]);
    if (db != K) {
      DUMP(db, i, ans[i], ans[j], ans[i] ^ ans[j]);
    }
    assert(db == K);
  }
  out("Yes");
  out_seq(ans);
  return true;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    bool res = (solve());
    if (not res) {
      out("No");
    }
  }
  exit_();
}
