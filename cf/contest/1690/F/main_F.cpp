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

using u64 = std::uint64_t;
struct RollingHash {
  using u128 = __uint128_t;
  static const u64 kMod = (1ULL << 61) - 1;

  static u64 base() {
    static const u64 kBase = []() {
      std::mt19937_64 engine(std::random_device{}());
      std::uniform_int_distribution<u64> rand(1000, kMod - 1);
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

  // Calculates hash(a || b) from hash(a), hash(b) and length(b).
  static u64 concat(u64 a_hash, u64 b_hash, int b_length) {
    return add(mul(a_hash, pow_base(b_length)), b_hash);
  }

  //  #include <atcoder/math>
  //  static u64 inv_base() {
  //    static const auto kInvBase = atcoder::inv_mod(
  //        (long long) base(), (long long) kMod);
  //    return kInvBase;
  //  }
};

// Computes hash for any substring in O(1).
struct SpanHash : public RollingHash {
  std::vector<u64> cum_hash;

  // Constructionn: O(n).
  // All elements must be non-zero. Otherwise we won't be able to distinguish
  // between [1] and [0, 1].
  template<class Sequence>
  explicit SpanHash(const Sequence &s) : cum_hash(s.size() + 1) {
    int i = 0;
    for (const auto &x: s) {
      u64 val = static_cast<u64>(x);
      assert(val != 0);  // Ensure all elements are non-zero!
      cum_hash[i + 1] = add(mul(cum_hash[i], base()), val);
      ++i;
    }
  }

  // Returns the hash value of the substring s[l:r]. O(1).
  u64 get(int l, int r) const {
    // Compute `hash(s[0:r]) - hash(s[0:l]) * B^(r-l) (mod M)`
    return sub(cum_hash[r], mul(cum_hash[l], pow_base(r - l)));
  }
};

// Returns all divisors of n. O(sqrt(n)) + sorting.
std::vector<int> divisors(int n) {
  std::vector<int> res;
  for (Int k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    Int q = n / k;
    if (q != k) res.push_back(q);
  }
  std::sort(res.begin(), res.end());
  res.pop_back();
  return res;
}

std::vector<int> ZArray(std::string_view s) {
  const int n = (int) s.size();
  std::vector<int> Z(n);
  Z[0] = n;
  int i = 1, j = 0;
  while (i < n) {
    while (i + j < n and s[j] == s[i + j]) ++j;
    Z[i] = j;
    if (j == 0) {
      ++i;
      continue;
    }
    int k = 1;
    for (; k < j and k + Z[k] < j; ++k) {
      Z[i + k] = Z[k];
    }
    i += k;
    j -= k;
  }
  return Z;
}

auto solve() {
  const int n = in;
  const string S = in;
  vector<int> P = in(n);
  for (auto &x: P) --x;

  vector<int> done(n, 0);
  Int ans = -1;
  REP(i, n) {
    if (done[i]) continue;
    done[i] = 1;
    string B(1, S[i]);
    for (int v = P[i]; v != i; v = P[v]) {
      done[v] = 1;
      B.push_back(S[v]);
    }
    int len = ssize(B);  // cycle length
    auto Z = ZArray(B);
    for (int d: divisors(len)) {
      bool ok = true;
      for (int j = d; j + d <= len; j += d) {
        if (Z[j] < d) {
          ok = false;
          break;
        }
      }
      if (ok) {
        len = d;
        break;
      }
    }
    DUMP(len);
    if (ans == -1) {
      ans = len;
    } else {
      ans = std::lcm(ans, len);
    }
  }
  print(ans);
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    dump_test_case(t, T);
    (solve());
  }
  exit_();
}
