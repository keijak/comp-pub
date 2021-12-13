#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <random>
#include <array>
#include <memory>
#include <cassert>

#include <boost/multiprecision/cpp_int.hpp>
using u128 = __uint128_t; //boost::multiprecision::uint128_t;
namespace multip = boost::multiprecision;

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T, int K = 12>
class PersistentArray {
 public:
  struct Node;
  // No memory release by default.
  using NodePtr = Node *;  // std::shared_ptr<Node>;

  struct Node {
    T val;
    std::unique_ptr<std::array<NodePtr, K>> children;
  };

  explicit PersistentArray(NodePtr root) : root_{std::move(root)} {}
  PersistentArray() : root_{} {}
  PersistentArray(const PersistentArray &) = default;
  PersistentArray(PersistentArray &&) = default;
  PersistentArray &operator=(const PersistentArray &) = default;
  PersistentArray &operator=(PersistentArray &&) = default;

  T operator[](int idx) const { return do_get(idx, root_); }

  PersistentArray<T, K> set(int idx, T val) const {
    return PersistentArray<T, K>(do_set(idx, val, root_));
  }

 private:
  static T do_get(int idx, const NodePtr &node) {
    if (node == nullptr) return T{};
    if (idx == 0) return node->val;
    if (node->children == nullptr) return T{};
    const NodePtr &child = (*node->children)[idx % K];
    if (child == nullptr) return T{};
    return do_get(idx / K, child);
  }

  static NodePtr do_set(int idx, T val, const NodePtr &node) {
    NodePtr res{new Node()};
    if (node != nullptr) {
      res->val = node->val;
      if (node->children != nullptr) {
        res->children.reset(new std::array<NodePtr, K>(*node->children));
      }
    }
    if (idx == 0) {
      res->val = std::move(val);
    } else {
      if (res->children == nullptr) {
        res->children.reset(new std::array<NodePtr, K>());
      }
      (*res->children)[idx % K] =
          do_set(idx / K, std::move(val), (*res->children)[idx % K]);
    }
    return res;
  }

 private:
  NodePtr root_;
};

using u64 = std::uint64_t;

struct RollingHash {
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

  // Calculates hash(s || t) from hash(s), hash(t) and len(t).
  static u64 concat(u64 a, u64 b, int b_length) {
    return add(mul(a, pow_base(b_length)), b);
  }
};

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  while (true_x - false_x > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

inline u128 encode(u64 hash, char ch) {
  return (u128(ch) << 64) | u128(hash);
}
inline u64 decode_hash(const u128 &val) {
  static u128 kMask = (u128(1) << 64) - 1;
  return u64(val & kMask);
}
inline char decode_char(const u128 &val) {
  return char(val >> 64);
}

constexpr int L = 1001;
PersistentArray<u128> fdp[L][L], bdp[L][L];

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  const int n = in;
  vector<string> M(n);
  REP(i, n) cin >> M[i];

  REP(i, n) {
    int pmink = 0;
    REP(j, n) {
      int pos = i + j;
      if (i == 0 and j == 0) {
        fdp[0][0] = fdp[0][0].set(0, encode(u64(M[0][0]), M[0][0]));
      } else if (i > 0 and j > 0) {
        auto &pa1 = fdp[i - 1][j];
        auto &pa2 = fdp[i][j - 1];
        u64 last1 = decode_hash(pa1[pos - 1]);
        u64 last2 = decode_hash(pa2[pos - 1]);
        if (last1 == last2) {
          u64 val = RollingHash::concat(last1, M[i][j], 1);
          fdp[i][j] = pa1.set(pos, encode(val, M[i][j]));
        } else {
          int mink = bisect<int>(pos - 1, min(pmink - 1, j), [&](int k) {
            return pa1[k] != pa2[k];
          });
          if (decode_char(pa1[mink]) <= decode_char(pa2[mink])) {
            u64 val = RollingHash::concat(last1, M[i][j], 1);
            fdp[i][j] = pa1.set(pos, encode(val, M[i][j]));
          } else {
            u64 val = RollingHash::concat(last2, M[i][j], 1);
            fdp[i][j] = pa2.set(pos, encode(val, M[i][j]));
          }
          pmink = mink;
        }
      } else if (i > 0) {
        u64 phash = decode_hash(fdp[i - 1][j][pos - 1]);
        u64 hash = RollingHash::concat(phash, M[i][j], 1);
        fdp[i][j] = fdp[i - 1][j].set(pos, encode(hash, M[i][j]));
      } else {
        u64 phash = decode_hash(fdp[i][j - 1][pos - 1]);
        u64 hash = RollingHash::concat(phash, M[i][j], 1);
        fdp[i][j] = fdp[i][j - 1].set(pos, encode(hash, M[i][j]));
      }
    }
  }

  REP(i, n) {
    const int ri = n - 1 - i;
    REP(j, n) {
      const int rj = n - 1 - j;
      const int rlen = i + j;
      const int pos = 2 * n - 2 - rlen;
      if (i == 0 and j == 0) {
        bdp[ri][rj] = bdp[ri][rj].set(pos, encode(u64(M[ri][rj]), M[ri][rj]));
      } else if (i > 0 and j > 0) {
        const auto &pa1 = bdp[ri + 1][rj];
        const auto &pa2 = bdp[ri][rj + 1];
        u64 last1 = decode_hash(pa1[pos + 1]);
        u64 last2 = decode_hash(pa2[pos + 1]);
        if (last1 == last2) {
          u64 hash = RollingHash::concat(M[ri][rj], last1, rlen);
          bdp[ri][rj] = pa1.set(pos, encode(hash, M[ri][rj]));
        } else {
          int mink;
          if (M[ri + 1][rj] != M[ri][rj + 1]) {
            mink = pos + 1;
          } else {
            mink = bisect<int>(2 * n - 1, pos + 1, [&](int k) {
              auto r1 = decode_hash(pa1[k]);
              auto r2 = decode_hash(pa2[k]);
              return RollingHash::sub(last1, r1) != RollingHash::sub(last2, r2);
            }) - 1;
          }
          if (decode_char(pa1[mink]) <= decode_char(pa2[mink])) {
            u64 hash = RollingHash::concat(M[ri][rj], last1, rlen);
            bdp[ri][rj] = pa1.set(pos, encode(hash, M[ri][rj]));
          } else {
            u64 hash = RollingHash::concat(M[ri][rj], last2, rlen);
            bdp[ri][rj] = pa2.set(pos, encode(hash, M[ri][rj]));
          }
        }
      } else if (i > 0) {
        u64 phash = decode_hash(bdp[ri + 1][rj][pos + 1]);
        u64 hash = RollingHash::concat(M[ri][rj], phash, rlen);
        bdp[ri][rj] = bdp[ri + 1][rj].set(pos, encode(hash, M[ri][rj]));
      } else {
        u64 phash = decode_hash(bdp[ri][rj + 1][pos + 1]);
        u64 hash = RollingHash::concat(M[ri][rj], phash, rlen);
        bdp[ri][rj] = bdp[ri][rj + 1].set(pos, encode(hash, M[ri][rj]));
      }
    }
  }

  const int Q = in;
  REP(i, Q) {
    const int x = int(in) - 1, y = int(in) - 1;
    int pos = x + y;
    auto &fa = fdp[x][y];
    auto &ba = bdp[x][y];
    for (int j = 0; j <= pos; ++j) {
      cout << decode_char(fa[j]);
    }
    for (int j = pos + 1; j <= 2 * n - 2; ++j) {
      cout << decode_char(ba[j]);
    }
    cout << '\n';
  }
}
