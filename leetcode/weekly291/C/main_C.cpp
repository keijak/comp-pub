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

struct RollingHash {
  using u64 = std::uint64_t;
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

class Solution {
 public:
  int countDistinct(vector<int> &nums, int k, int p) {
    const int n = nums.size();
    SpanHash sh(nums);
    set<SpanHash::u64> ans;
    //set<vector<int>> ans;
    for (int r = 1; r <= n; ++r) {
      vector<int> sub;
      int pcount = 0;
      for (int l = r - 1; l >= 0; --l) {
        int val = nums[l];
        if (val % p == 0) {
          ++pcount;
        }
        if (pcount > k) break;
        sub.push_back(val);
        //DUMP(sub);
        //ans.insert(sub);
        ans.insert(sh.get(l, r));
      }
    }
    return ans.size();
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  Solution sol;
  vector<int> vec = {2, 3, 3, 2, 2};
  auto ans = sol.countDistinct(vec, 2, 2);
  DUMP(ans);

  exit_();
}
