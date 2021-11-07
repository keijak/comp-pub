#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;
using u64 = std::uint64_t;

struct RollingHash {
  using u64 = std::uint64_t;
  using u128 = __uint128_t;
  static const u64 kMod = (1ULL << 61) - 1;

  static u64 base() {
    static const auto kBase = []() -> u64 {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, kMod - 1);
      return rand(engine);
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= kMod) ? (a - kMod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, kMod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & kMod);
    na += nb;
    return (na >= kMod) ? (na - kMod) : na;
  }

  static u64 pow_base(int i) {
    static std::vector<u64> kPowBase(1, 1);
    while (int(kPowBase.size()) <= i) {
      u64 val = mul(kPowBase.back(), base());
      kPowBase.push_back(val);
    }
    return kPowBase[i];
  }
};

// Computes hash for any substring in O(1).
struct SpanHash : public RollingHash {
  std::vector<u64> cum_hash;

  // Constructionn: O(n).
  // All elements must be non-zero. Otherwise we cannot distinguish between [1] and [0, 1].
  template<class Seq>
  explicit SpanHash(const Seq &s) : cum_hash(s.size() + 1) {
    const int n = s.size();
    int i = 0;
    for (const auto &x: s) {
      u64 val = static_cast<u64>(x);
      assert(val != 0); // Ensure that all values are non-zero!
      cum_hash[i + 1] = add(mul(cum_hash[i], base()), val);
      ++i;
    }
  }

  // Returns the hash value of the substring s[l:r]. O(1).
  u64 get(int l, int r) const {
    // Compute `hash(s[0:r]) - hash(s[0:l]) * B^(r-l) (mod M)`
    return add(cum_hash[r], kMod - mul(cum_hash[l], pow_base(r - l)));
  }
};

auto solve() {
  int n = in;
  set<u64> hs;
  REP(i, n) {
    int l = in;
    vector<int> a = in(l);
    for (auto &e: a) ++e;
    SpanHash sh(a);
    hs.insert(sh.cum_hash[l]);
  }
  return hs.size();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
