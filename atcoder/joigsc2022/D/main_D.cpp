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

template<size_t BufSize>
class StdinReader {
 public:
  StdinReader() : p{buf} {
    const size_t len = fread/* _unlocked */(buf, 1, sizeof(buf) - 1, stdin);
    buf[len] = '\0';
    bufend = buf + len;
  }

  void skip() { while (isspace(*p)) ++p; }

  bool is_eof() { return p >= bufend; }

  template<typename T>
  operator T() {
    T x;
    skip();
    assert(not is_eof());  // Couldn't read reached the end of input.
    read_one(x);
    return x;
  }

  struct Sized {
    StdinReader &reader;
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        reader.skip();
        assert(not reader.is_eof());
        read(x);
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {*this, n}; }

 private:
  static inline char buf[BufSize];
  char *p, *bufend;

  template<class T>
  std::enable_if_t<std::is_integral_v<T>> read_one(T &x) {
    [[maybe_unused]] int sign = 1;
    if constexpr (std::is_signed_v<T>) {
      sign = (*p == '-') ? (++p, -1) : 1;
    }
    x = 0;
    while (isdigit(*p)) x = x * 10 + (*p++ & 0x0F);
    if constexpr (std::is_signed_v<T>) {
      x *= sign;
    }
  }
  void read_one(std::string &s) {
    char *p0 = p;
    while (not isspace(*p)) p++;
    s.assign(p0, p);
  }
  void read_one(std::string_view &s) {
    const char *p0 = p;
    while (not isspace(*p)) p++;
    s = std::string_view(p0, p - p0);
  }
};
StdinReader<(1 << 26)> in;

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
  in.skip();
  assert(in.is_eof());  // Some input is left.
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

auto solve() {
  const int n = in;
  const string_view S = in;
  const Int A = in, B = in, C = in;
  assert(ssize(S) == n);
  DUMP(A, B, C);
  SpanHash sh(S);
  auto nex = vector(n + 1, vector(n + 1, -1));
  for (int len = 1; len <= n; ++len) {
    map<u64, deque<int>> ix;
    deque<pair<int, u64>> q;
    for (int l = n - len; l >= 0; --l) {
      const int r = l + len;
      u64 h = sh.get(l, r);
      q.emplace_back(l, h);
      while (not q.empty() and q.front().first >= l + len) {
        auto [j, hp] = q.front();
        q.pop_front();
        ix[hp].push_front(j);
      }
      auto &indexes = ix[h];
      if (not indexes.empty()) {
        nex[l][r] = indexes.front();
      }
    }
  }
  auto dp = vector(n + 1, vector(n + 1, kBig<Int>));
  REP(i, n + 1) dp[i][i] = 0;
  REP(i, n) dp[i][i + 1] = A;
  for (int len = 1; len <= n; ++len) {
    for (int l = n - len; l >= 0; --l) {
      const int r = l + len;
      if (dp[l][r] == kBig<Int>) continue;
      if (r + 1 <= n) {
        chmin(dp[l][r + 1], dp[l][r] + A);
      }
      if (l - 1 >= 0) {
        chmin(dp[l - 1][r], dp[l][r] + A);
      }
      for (int l2 = nex[l][r], cnt = 2; l2 >= 0; ++cnt, l2 = nex[l2][l2 + len]) {
        const int r2 = l2 + len;
        const int total = r2 - l;
        const Int pasted = cnt * len;
        const Int fill = total - pasted;
        chmin(dp[l][r2], dp[l][r] + B + cnt * C + fill * A);
      }
    }
  }
  out(dp[0][n]);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
