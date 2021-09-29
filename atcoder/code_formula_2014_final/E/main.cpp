#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
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
    is_iterable<T>::value && !std::is_same<T, std::string_view>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

struct RollingHash {
  static const u64 mod = (1ULL << 61) - 1;
  std::vector<u64> hash_val, base_power;

  explicit RollingHash(std::string_view s)
      : hash_val(s.size() + 1), base_power(s.size() + 1) {
    const int n = s.size();
    base_power[0] = 1;
    for (int i = 0; i < n; i++) {
      base_power[i + 1] = mul(base_power[i], base());
      hash_val[i + 1] = add(mul(hash_val[i], base()), u64(s[i]));
    }
  }

  // Returns the hash value for the [l,r) interval.
  u64 get(int l, int r) {
    return add(hash_val[r], mod - mul(hash_val[l], base_power[r - l]));
  }

  static u64 base() {
    static u64 val = []() -> u64 {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, mod - 1);
      return rand(engine);
    }();
    return val;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= mod) ? (a - mod) : a;
  }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = (u128) a * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & mod);
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};

auto solve() -> pair<i64, i64> {
  const string s = in;
  const int slen = ssize(s);
  if (s == "b") return {1, 0};
  if (s == "a") return {2, 0};

  vector<int> fibs = {0, 1};
  for (int i = 2;; ++i) {
    fibs.push_back(fibs[i - 1] + fibs[i - 2]);
    if (fibs[i] >= slen) break;
  }
  assert(fibs.back() == slen);
  const int p = ssize(fibs) - 1;
  DUMP(p);
  RollingHash hasher(s);
  const auto shash = hasher.get(0, slen);

  map<pair<int, int>, optional<u64>> memo;
  auto f = [&](auto &f, int n, int k) -> optional<u64> {
    if (n == 1 and k == 0) {
      return 'b';
    } else if (n == 2 and k == 0) {
      return 'a';
    } else if (n < 3) {
      return nullopt;
    }
    auto mkey = pair{n, k};
    if (auto it = memo.find(mkey); it != memo.end()) return it->second;
    auto h1 = f(f, n - 1, k / 2);
    auto h2 = f(f, n - 2, k / 4);
    if (not h1 or not h2) return nullopt;
    if (k % 2 == 0) {
      auto ret = RollingHash::add(RollingHash::mul(*h1, hasher.base_power[fibs[n - 2]]), *h2);
      return memo[mkey] = ret;
    } else {
      auto ret = RollingHash::add(RollingHash::mul(*h2, hasher.base_power[fibs[n - 1]]), *h1);
      return memo[mkey] = ret;
    }
  };

  for (int q = 0;; ++q) {
    auto h = f(f, p, q);
    if (not h) continue;
    if (*h == shash) {
      return {p, q};
    }
  }
  assert(false);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto[p, q] = solve();
    print(p, q);
  }
}
