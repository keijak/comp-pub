#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
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

 private:
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
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};

bool inc(string &t) {
  for (int i = ssize(t) - 1; i >= 0; --i) {
    if (t[i] < 'z') {
      t[i]++;
      for (int j = i + 1; j < ssize(t); ++j) {
        t[j] = 'a';
      }
      return true;
    }
  }
  return false;
}

string solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  assert(ssize(s) == n);
  RollingHash rhs(s);
  for (int len = 1;; ++len) {
    string t(len, 'a');
    i64 m = 1;
    REP(i, len) { m *= 26; }
    REP(i, m) {
      RollingHash rht(t);
      auto th = rht.get(0, len);
      bool found = false;
      for (int j = 0; j + len <= n; ++j) {
        auto sh = rhs.get(j, j + len);
        if (sh == th) {
          found = true;
          break;
        }
      }
      if (not found) return t;
      inc(t);
    }
  }
  assert(false);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) cout << solve() << "\n";
}
